#include "Compare.hpp"
#include "HashInventory.hpp"
#include "Inventory.hpp"
#include "Item.hpp"
#include "ItemAVL.hpp"
#include "ItemGenerator.hpp"
#include "TreeInventory.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <vector>

// ─── Timing helpers ────────────────────────────────────────────────────────

// Populates any Inventory with n random items from a fresh generator (seed 42)
template <class Inv>
void populate(Inv& inv, int n)
{
    ItemGenerator gen(42);
    for (int i = 0; i < n; i++) inv.pickup(gen.randomItem());
}

// Times contains() over 100 contained + 100 missing names (200 calls total)
// Returns average time in milliseconds
template <class Comparator, class Container>
double timeContains(int n)
{
    Inventory<Comparator, Container> inv;
    ItemGenerator gen(42);

    // Insert n items
    for (int i = 0; i < n; i++) inv.pickup(gen.randomItem());

    // 100 names known to be in the inventory
    std::vector<std::string> contained;
    for (int i = 0; i < 100; i++) contained.push_back(gen.randomUsedName());

    // 100 names from new items NOT inserted
    std::vector<std::string> missing;
    for (int i = 0; i < 100; i++) missing.push_back(gen.randomItem().name_);

    double total_ms = 0.0;

    for (const std::string& name : contained) {
        auto t1 = std::chrono::high_resolution_clock::now();
        inv.contains(name);
        auto t2 = std::chrono::high_resolution_clock::now();
        total_ms += std::chrono::duration<double, std::milli>(t2 - t1).count();
    }
    for (const std::string& name : missing) {
        auto t1 = std::chrono::high_resolution_clock::now();
        inv.contains(name);
        auto t2 = std::chrono::high_resolution_clock::now();
        total_ms += std::chrono::duration<double, std::milli>(t2 - t1).count();
    }

    return total_ms / 200.0;
}

// Times query() 10 times using name comparator
template <class Comparator, class Container>
double timeQueryName(int n)
{
    Inventory<Comparator, Container> inv;
    ItemGenerator gen(42);
    for (int i = 0; i < n; i++) inv.pickup(gen.randomItem());

    double total_ms = 0.0;
    for (int i = 0; i < 10; i++) {
        std::string nameA = gen.randomUsedName();
        std::string nameB = gen.randomUsedName();
        Item start(nameA, 0, NONE);
        Item end(nameB, 0, NONE);
        // Ensure start <= end lexicographically
        if (Comparator::lessThan(end, start)) std::swap(start, end);

        auto t1 = std::chrono::high_resolution_clock::now();
        inv.query(start, end);
        auto t2 = std::chrono::high_resolution_clock::now();
        total_ms += std::chrono::duration<double, std::milli>(t2 - t1).count();
    }
    return total_ms / 10.0;
}

// Times query() 10 times using weight comparator (narrow 0.1-wide window)
template <class Comparator, class Container>
double timeQueryWeight(int n)
{
    Inventory<Comparator, Container> inv;
    ItemGenerator gen(42);
    for (int i = 0; i < n; i++) inv.pickup(gen.randomItem());

    double total_ms = 0.0;
    for (int i = 0; i < 10; i++) {
        float w = gen.randomFloat(ItemGenerator::MIN_WEIGHT, ItemGenerator::MAX_WEIGHT);
        Item start("s", w, NONE);
        Item end("e", w + 0.1f, NONE);

        auto t1 = std::chrono::high_resolution_clock::now();
        inv.query(start, end);
        auto t2 = std::chrono::high_resolution_clock::now();
        total_ms += std::chrono::duration<double, std::milli>(t2 - t1).count();
    }
    return total_ms / 10.0;
}

// ─── Print helpers ──────────────────────────────────────────────────────────

void printRow(const std::string& label, const std::vector<double>& vals)
{
    std::cout << std::left << std::setw(22) << label;
    for (double v : vals)
        std::cout << std::right << std::setw(14) << std::fixed << std::setprecision(6) << v;
    std::cout << "\n";
}

void printHeader(const std::vector<int>& ns)
{
    std::cout << std::left << std::setw(22) << "Container";
    for (int n : ns)
        std::cout << std::right << std::setw(14) << ("n=" + std::to_string(n));
    std::cout << "\n" << std::string(22 + 14 * ns.size(), '-') << "\n";
}

// ─── Main ───────────────────────────────────────────────────────────────────

int main()
{
    const std::vector<int> ns = { 1000, 2000, 4000, 8000 };

    // ── Task 4A: contains() timing ──────────────────────────────────────────
    std::cout << "\n=== Task 4A: Average contains() time (ms) ===\n";
    std::cout << "(Comparator: CompareItemName, 200 calls per trial)\n\n";
    printHeader(ns);

    {
        std::vector<double> row;
        for (int n : ns) row.push_back(timeContains<CompareItemName, std::vector<Item>>(n));
        printRow("vector", row);
    }
    {
        std::vector<double> row;
        for (int n : ns) row.push_back(timeContains<CompareItemName, std::list<Item>>(n));
        printRow("list", row);
    }
    {
        std::vector<double> row;
        for (int n : ns) row.push_back(timeContains<CompareItemName, std::unordered_set<Item>>(n));
        printRow("unordered_set (hash)", row);
    }
    {
        std::vector<double> row;
        for (int n : ns) row.push_back(timeContains<CompareItemName, Tree>(n));
        printRow("AVL tree", row);
    }

    // ── Task 4B Test 1: query() by name ────────────────────────────────────
    std::cout << "\n=== Task 4B Test 1: Average query() time (ms) ===\n";
    std::cout << "(Comparator: CompareItemName, 10 queries per trial)\n\n";
    printHeader(ns);

    {
        std::vector<double> row;
        for (int n : ns) row.push_back(timeQueryName<CompareItemName, std::vector<Item>>(n));
        printRow("vector", row);
    }
    {
        std::vector<double> row;
        for (int n : ns) row.push_back(timeQueryName<CompareItemName, std::list<Item>>(n));
        printRow("list", row);
    }
    {
        std::vector<double> row;
        for (int n : ns) row.push_back(timeQueryName<CompareItemName, std::unordered_set<Item>>(n));
        printRow("unordered_set (hash)", row);
    }
    {
        std::vector<double> row;
        for (int n : ns) row.push_back(timeQueryName<CompareItemName, Tree>(n));
        printRow("AVL tree", row);
    }

    // ── Task 4B Test 2: query() by weight ──────────────────────────────────
    std::cout << "\n=== Task 4B Test 2: Average query() time (ms) ===\n";
    std::cout << "(Comparator: CompareItemWeight, 10 queries per trial)\n\n";
    printHeader(ns);

    {
        std::vector<double> row;
        for (int n : ns) row.push_back(timeQueryWeight<CompareItemWeight, std::vector<Item>>(n));
        printRow("vector", row);
    }
    {
        std::vector<double> row;
        for (int n : ns) row.push_back(timeQueryWeight<CompareItemWeight, std::list<Item>>(n));
        printRow("list", row);
    }
    {
        std::vector<double> row;
        for (int n : ns) row.push_back(timeQueryWeight<CompareItemWeight, std::unordered_set<Item>>(n));
        printRow("unordered_set (hash)", row);
    }
    {
        std::vector<double> row;
        for (int n : ns) row.push_back(timeQueryWeight<CompareItemWeight, Tree>(n));
        printRow("AVL tree", row);
    }

    return 0;
}
