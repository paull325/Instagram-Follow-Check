#include "CustomString.h"

#include "Database/FollowerDatabase.h"

#include <memory>
#include <string>
#include <fstream>

//not only search, but also write in table
void searchFile(const std::unique_ptr<db::FollowerDatabase>& db, const std::string& file, const std::string& search)
{
    CustomString line;
    CustomString name;

    std::size_t foundNext;

    std::ifstream followerFile(file);

    if (followerFile.is_open())
    {
        while (getline(followerFile, line.content))
        {
            if (line.content.find(search) != std::string::npos)
            {
                foundNext = line.findNext(search);

                while (foundNext != std::string::npos)
                {
                    name.content = line.section((unsigned int)foundNext + 16, '"');         //16 characters after unique keyword the username starts
                    if (file == "follower.htm")
                    {
                        db->AddFollower(name.content);
                    }
                    else if (file == "following.htm")
                    {
                        db->AddFollowing(name.content);
                    }
                    foundNext = line.findNext(search);
                }
                line.resetSearchPos();
            }
        }
        followerFile.close();
    }
    else
    {
        std::cout << "Unable to open file\n";
    }
}

int main() 
{ 
    const auto followerDB = db::FollowerDatabase::OpenDatabase("names.db");

    if (!followerDB)
    {
        return 1;
    }

    int select;
    bool running = true;

    do 
    {
        std::cout << "What would you like to do?" << std::endl
            << "1) Refresh follower data" << std::endl
            << "2) Display following, who do not follow back" << std::endl
            << "3) Display followers, whom you are not following back" << std::endl
            << "4) Exit" << std::endl;
        std::cin >> select;

        switch (select) 
        {
            case 1:
                followerDB->RecreateTables();

                searchFile(followerDB, "follower.htm", "_0imsa");
                searchFile(followerDB, "following.htm", "_0imsa");
                std::cout << "Refresh complete." << std::endl;
                break;
            case 2:
                followerDB->PrintAllPeopleNotFollowingBack();
                break;
            case 3:
                followerDB->PrintAllPeopleYouAreNotFollowingBack();
                break;
            case 4:
                running = false;
                break;
            default:
                std::cout << "Invalid option, Please enter option (1-4).";
                if (!std::cin.good()) 
                {
                    std::cin.clear();
                    std::cin.ignore(INT_MAX, '\n');
                }
                break;
        }
    } while (running);

    return 0;
}