#pragma once

// Owner: Member 5 (Storage Module)

#include <string>
#include <vector>
#include "../models/Book.h"
#include "../models/User.h"
#include "../models/Admin.h"

class StorageService {
public:
static bool saveBooks(const std::vector<Book>& books);
static bool loadBooks(std::vector<Book>& books);

static bool saveUsers(const std::vector<User>& users);
static bool loadUsers(std::vector<User>& users);

static bool saveAdmins(const std::vector<Admin>& admins);
static bool loadAdmins(std::vector<Admin>& admins);
};