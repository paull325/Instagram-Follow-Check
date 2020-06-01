#include "FollowerDatabase.h"

#include <fmt/format.h>
#include <sqlite3/sqlite3.h>

#include <iostream>

namespace
{

struct Table
{
	const std::string name;
	const std::string columns;
	const std::string insertScript;
};

namespace Tables
{

static const Table Followers{ "Followers", "Name TEXT", "INSERT INTO Followers (Name) VALUES ('{0}');" };
static const Table Following{ "Following", "Name TEXT", "INSERT INTO Following (Name) VALUES ('{0}');" };

} // namespaces TableName

namespace SQLScriptTemplate
{

static const std::string DropAndRecreateTable = "DROP TABLE IF EXISTS {0}; CREATE TABLE {0}({1});";

static const std::string ExclusiveEntries = "SELECT Name FROM {0} EXCEPT SELECT Name FROM {1};";

} // namespaces SQLScriptTemplate
} // anonymous namespace


namespace db
{

std::unique_ptr<FollowerDatabase> FollowerDatabase::OpenDatabase(const std::string& databaseFileName)
{
	sqlite3* database = nullptr;
	const auto resultCode = sqlite3_open(databaseFileName.c_str(), &database);

	if (resultCode != SQLITE_OK)
	{
		std::cerr << "Can't open database: " << sqlite3_errmsg(database) << std::endl;
		return nullptr;
	}

	if (database == nullptr)
	{
		std::cerr << "Invalid state for database!" << std::endl;
		return nullptr;
	}

	return std::unique_ptr<FollowerDatabase>(new FollowerDatabase(database));
}


FollowerDatabase::FollowerDatabase(sqlite3* database)
	: m_database(database)
{}

FollowerDatabase::~FollowerDatabase()
{
	sqlite3_close(m_database);
}

bool FollowerDatabase::RecreateTables()
{
	const std::string rectreateFollowersTableScript = fmt::format(SQLScriptTemplate::DropAndRecreateTable, Tables::Followers.name, Tables::Followers.columns);

	const std::string rectreateFollowingTableScript = fmt::format(SQLScriptTemplate::DropAndRecreateTable, Tables::Following.name, Tables::Following.columns);

	return ExecuteSQLStatement(rectreateFollowersTableScript + rectreateFollowingTableScript);
}

bool FollowerDatabase::PrintAllPeopleNotFollowingBack()
{
	const std::string followingNotFollowingBack = fmt::format(SQLScriptTemplate::ExclusiveEntries, Tables::Following.name, Tables::Following.name);
	return ExecuteSQLStatement(followingNotFollowingBack);
}

bool FollowerDatabase::PrintAllPeopleYouAreNotFollowingBack()
{
	const std::string followersYouAreNotFollowingBack = fmt::format(SQLScriptTemplate::ExclusiveEntries, Tables::Following.name, Tables::Following.name);
	return ExecuteSQLStatement(followersYouAreNotFollowingBack);
}

bool FollowerDatabase::AddFollower(const std::string& name)
{
	const std::string addNewFollowerScript = fmt::format(Tables::Followers.insertScript, name);
	return ExecuteSQLStatement(addNewFollowerScript);
}

bool FollowerDatabase::AddFollowing(const std::string& name)
{
	const std::string addNewFollowingScript = fmt::format(Tables::Following.insertScript, name);
	return ExecuteSQLStatement(addNewFollowingScript);
}

bool FollowerDatabase::ExecuteSQLStatement(const std::string& statement)
{
	const auto sqlCallback = [](void*, int count, char** data, char** columns) -> int
	{
		for (int i = 0; i < count; ++i)
		{
			const std::string valueOrNull = data[i] != 0 ? data[i] : "NULL";
			std::cout << fmt::format("{} = {}", columns[i], valueOrNull) << std::endl;
		}
		std::cout << std::endl;
		return 0;
	};

	char* errorMessage;
	const auto resultCode = sqlite3_exec(m_database, statement.c_str(), sqlCallback, nullptr, &errorMessage);

	if (resultCode != SQLITE_OK) 
	{
		std::cerr << "Failed to execute SQL statement! " << errorMessage << std::endl;
		sqlite3_free(errorMessage);
		return false;
	}

	return true;
}

} // namespace db