#pragma once
#include <gmpxx.h>
#include <string>
#include <sqlite3.h>
#include <vector>

class StorageEngine {
private:
    sqlite3* db;
    std::string db_path;
    sqlite3_stmt* insert_stmt;
    sqlite3_stmt* select_stmt;

public:
    explicit StorageEngine(const std::string& path = "mitm_space.db");
    ~StorageEngine();

    void clear();
    void begin_transaction();
    void end_transaction();
    void insert_sum(const mpz_class& sum, const std::vector<bool>& mask);
    bool find_complement(const mpz_class& target, std::vector<bool>& out_mask);
};