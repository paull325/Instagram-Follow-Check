#include <fstream>
#include <sqlite3.h>

#include "CustomString.h"

using String = std::string;

sqlite3* db;
char* zErrMsg = 0;
int rc;

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void sqlexec(String command) {                                                          /* Execute SQL statement */

    rc = sqlite3_exec(db, command.c_str(), callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
       // std::cout << "Succesful, Command: " << command << "\n";
    }
}

void writeInTable(String tableName, String content) {
    String sql = "INSERT INTO " + tableName + " (NAME) "  \
        "VALUES ('" + content + "'); ";

    sqlexec(sql);
}

void searchFile(String file, String search) {       //not only search, but also write in table

    CustomString line;
    CustomString name;

    std::size_t foundNext;

    std::ifstream followerFile(file);

    if (followerFile.is_open())
    {
        while (getline(followerFile, line.content))
        {
            if (line.content.find(search) != std::string::npos) {

                foundNext = line.findNext(search);

                while (foundNext != std::string::npos) {
                    name.content = line.section((unsigned int)foundNext + 16, '"');         //16 characters after unique keyword the username starts
                    if(file== "follower.htm")
                        writeInTable("Followers", name.content);
                    else if (file == "following.htm")
                        writeInTable("Following", name.content);
                    foundNext = line.findNext(search);
                }
                line.resetSearchPos();
            }
        }
        followerFile.close();
    }
    else std::cout << "Unable to open file\n";
}

int main() { 

    /* Open database */
    rc = sqlite3_open("names.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n\n", sqlite3_errmsg(db));
        return(0);
    }
    else {
        fprintf(stdout, "Opened database successfully\n\n");
    }

    int select;

    do {
        std::cout << "\nWhat would you like to do?\n 1) Refresh Database\n 2) Display following, who do not follow back\n 3) Display followers, whom you are not following back\n 4) Exit\n";
        std::cin >> select;

        switch (select) {
            case 1:
                sqlexec("DROP TABLE Followers;");
                sqlexec("DROP TABLE Following;");
                sqlexec("CREATE TABLE IF NOT EXISTS Followers("  \
                    "Name        TEXT);");
                sqlexec("CREATE TABLE IF NOT EXISTS Following("  \
                    "Name        TEXT);");

                searchFile("follower.htm", "_0imsa");
                searchFile("following.htm", "_0imsa");
                std::cout << "Refresh complete.";
                break;
            case 2:
                sqlexec("SELECT Name FROM Following EXCEPT SELECT Name FROM Followers;");
                break;
            case 3:
                sqlexec("SELECT Name FROM Followers EXCEPT SELECT Name FROM Following;");
                break;
            case 4:
                break;
            default:
                std::cout << "Invalid option, Please enter option (1-4).";
                break;
        }
    } while (select != 4);

    /* Close database */
    sqlite3_close(db);

    return 0;
}