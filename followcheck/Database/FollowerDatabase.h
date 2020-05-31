#pragma once

#include <string>
#include <memory>

// sqlite forward declaraion
struct sqlite3;

namespace db
{

class FollowerDatabase
{
public:
	static std::unique_ptr<FollowerDatabase> OpenDatabase(const std::string& databaseFileName);

	~FollowerDatabase();

	bool RecreateTables();

	bool PrintAllPeopleNotFollowingBack();
	bool PrintAllPeopleYouAreNotFollowingBack();

	bool AddFollower(const std::string& name);
	bool AddFollowing(const std::string& name);

private:
	FollowerDatabase(sqlite3* database);

	bool ExecuteSQLStatement(const std::string& statement);

	sqlite3* m_database;
};

} // namespace db