#include "storage_engine.hpp"
#include <stdexcept>
#include <iostream>

StorageEngine::StorageEngine(const std::string& path) : db_path(path), db(nullptr), insert_stmt(nullptr), select_stmt(nullptr) {
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Failed to open SQLite DB");
    }
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "PRAGMA synchronous=OFF;", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "PRAGMA cache_size=-64000;", nullptr, nullptr, nullptr);

    std::string create_table = "CREATE TABLE IF NOT EXISTS left_space (sum TEXT PRIMARY KEY, mask BLOB);";
    sqlite3_exec(db, create_table.c_str(), nullptr, nullptr, nullptr);

    sqlite3_prepare_v2(db, "INSERT OR IGNORE INTO left_space VALUES (?, ?);", -1, &insert_stmt, nullptr);
    sqlite3_prepare_v2(db, "SELECT mask FROM left_space WHERE sum = ?;", -1, &select_stmt, nullptr);
}

StorageEngine::~StorageEngine() {
    sqlite3_finalize(insert_stmt);
    sqlite3_finalize(select_stmt);
    sqlite3_close(db);
}

void StorageEngine::clear() {
    sqlite3_exec(db, "DELETE FROM left_space;", nullptr, nullptr, nullptr);
}

void StorageEngine::begin_transaction() {
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
}

void StorageEngine::end_transaction() {
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
}

void StorageEngine::insert_sum(const mpz_class& sum, const std::vector<bool>& mask) {
    std::string sum_str = sum.get_str();
    std::vector<uint8_t> blob;
    uint8_t current_byte = 0;
    for (size_t i = 0; i < mask.size(); ++i) {
        if (mask[i]) current_byte |= (1 << (i % 8));
        if ((i + 1) % 8 == 0 || i == mask.size() - 1) {
            blob.push_back(current_byte);
            current_byte = 0;
        }
    }

    sqlite3_bind_text(insert_stmt, 1, sum_str.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_blob(insert_stmt, 2, blob.data(), static_cast<int>(blob.size()), SQLITE_TRANSIENT);

    sqlite3_step(insert_stmt);
    sqlite3_reset(insert_stmt);
}

bool StorageEngine::find_complement(const mpz_class& target, std::vector<bool>& out_mask) {
    std::string target_str = target.get_str();
    sqlite3_bind_text(select_stmt, 1, target_str.c_str(), -1, SQLITE_TRANSIENT);

    bool found = false;
    if (sqlite3_step(select_stmt) == SQLITE_ROW) {
        found = true;
        const uint8_t* blob = static_cast<const uint8_t*>(sqlite3_column_blob(select_stmt, 0));
        int bytes = sqlite3_column_bytes(select_stmt, 0);

        out_mask.resize(bytes * 8);
        for (int i = 0; i < bytes * 8; ++i) {
            out_mask[i] = (blob[i / 8] & (1 << (i % 8))) != 0;
        }
    }
    sqlite3_reset(select_stmt);
    return found;
}