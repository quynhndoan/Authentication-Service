#include <iostream>
#include <string>
#include <sstream>
#include "HashMap.hpp"

int main()
{
    bool debugSign = false;     // Assume debug is off
    std::string userInputLine;
    std::string userCommand;
    std::string userEmail;
    std::string userPassword;
    HashMap userLogins = HashMap();
    while (true) {
        std::getline(std::cin, userInputLine);
        // Remove all username/password combinations from the service
        if (userInputLine == "QUIT")
        {
            std::cout << "GOODBYE" << std::endl;
            break;
        }
        else if (userInputLine == "CLEAR")
        {
            userLogins = HashMap();
            std::cout << "CLEARED" << std::endl;
            break;
        }

        // Makes the other debug commands available
        else if (userInputLine == "DEBUG ON")
        {
            if (debugSign)
            {
                std::cout << "ON ALREADY" << std::endl;
            }
            else
            {
                debugSign = true;
                std::cout << "ON NOW" << std::endl;
            }
        }

        // Makes the other debug commands unavailable
        else if (userInputLine == "DEBUG OFF")
        {
            if (debugSign)
            {
                debugSign = false;
                std::cout << "OFF NOW" << std::endl;
            }
            else
            {
                std::cout << "OFF ALREADY" << std::endl;
            }
        }

        // The output number of username/password combinations currently being stored
        else if (userInputLine == "LOGIN COUNT")
        {
            if (debugSign)
            {
                std::cout << userLogins.size() << std::endl;
            }
            else
            {
                std::cout << "INVALID" << std::endl;
            }
        }

        // The output is the number of buckets in the hash table at present
        else if (userInputLine == "BUCKET COUNT")
        {
            if (debugSign)
            {
                std::cout << userLogins.bucketCount() << std::endl;
            }
            else
            {
                std::cout << "INVALID" << std::endl;
            }
        }

        // The output is the load factor of the hash table at representing
        else if (userInputLine == "LOAD FACTOR")
        {
            if (debugSign)
            {
                std::cout << userLogins.loadFactor() << std::endl;
            }
            else
            {
                std::cout << "INVALID" << std::endl;
            }
        }

        // The output is the length of the largets buckets

        else if (userInputLine == "MAX BUCKET SIZE")
        {
            if (debugSign)
            {
                std::cout << userLogins.maxBucketSize() << std::endl;
            }
            else
            {
                std::cout << "INVALID" << std::endl;
            }
        }
        else
        {
            std::istringstream stream(userInputLine);
            // Intinialize all user input variables
            userCommand = "";
            userEmail = "";
            userPassword = "";
            stream >> userCommand;
            if (userCommand == "CREATE")
            {
                stream >> userEmail >> userPassword;
                // If user leave this line blank then output INVALID
                if (userEmail == "" || userPassword == "")
                {
                    std::cout << "INVALID" << std::endl;
                }
                // If the username is already stored in the collection, and no changed
                // is made and the output is EXISTS.
                else if (userLogins.contains(userEmail))
                {
                    std::cout << "EXISTS" << std::endl;
                }
                else
                {
                    userLogins.add(userEmail, userPassword);
                    std::cout << "CREATED" << std::endl;
                }
            }
            else if (userCommand == "LOGIN")
            {
                stream >> userEmail >> userPassword;
                if (userEmail == "" || userPassword == "")
                {
                    std::cout << "INVALID" << std::endl;
                }
                // If the username is already stored in the collection, and no changed
                // is made and the output is EXISTS.
                else if (userLogins.value(userEmail) == userPassword)
                {
                    std::cout << "SUCCEEDED" << std::endl;
                }
                else
                {
                    std::cout << "FAILED" << std::endl;
                }
            }
            else if (userCommand == "REMOVE")
            {
                stream >> userEmail;
                if (userEmail == "")
                {
                    std::cout << "INVALID" << std::endl;
                }
                else if (userLogins.contains(userEmail))
                {
                    userLogins.remove(userEmail);
                    std::cout << "REMOVED" << std::endl;
                }
                else
                {
                    std::cout << "NONEXISTENT" << std::endl;
                }
            }
            else
            {
                std::cout << "INVALID" << std::endl;
            }
        }
    }


    return 0;
}
