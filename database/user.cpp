#include "user.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void User::init()
    {
        try
        {
            for (auto &hint : database::Database::get_all_hints())
            {

                Poco::Data::Session session = database::Database::get().create_session();
                Statement create_stmt(session);
                create_stmt << "CREATE TABLE IF NOT EXISTS `User` (`id` INT NOT NULL,"
                            << "`first_name` VARCHAR(256) NOT NULL,"
                            << "`last_name` VARCHAR(256) NOT NULL,"
                            << "`login` VARCHAR(256) NOT NULL,"
                            << "`password` VARCHAR(256) NOT NULL,"
                            << "`email` VARCHAR(256) NULL,"
                            << "`role` VARCHAR(1024) NULL,"
                            << "PRIMARY KEY (`id`),KEY `fn` (`first_name`),KEY `ln` (`last_name`));"
                            << hint,
                            now;
            std::cout << create_stmt.toString() << std::endl;
            }
            
            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE SEQUENCE IF NOT EXISTS ids START WITH 1 INCREMENT BY 1;",
            now;
            std::cout << create_stmt.toString() << std::endl;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }


    Poco::JSON::Object::Ptr User::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("first_name", _first_name);
        root->set("last_name", _last_name);
        root->set("email", _email);
        root->set("role", _role);
        root->set("login", _login);
        root->set("password", _password);

        return root;
    }

    User User::fromJSON(const std::string &str)
    {
        User user;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        user.id() = object->getValue<long>("id");
        user.first_name() = object->getValue<std::string>("first_name");
        user.last_name() = object->getValue<std::string>("last_name");
        user.email() = object->getValue<std::string>("email");
        user.role() = object->getValue<std::string>("role");
        user.login() = object->getValue<std::string>("login");
        user.password() = object->getValue<std::string>("password");

        return user;
    }

    std::optional<long> User::auth(std::string &login, std::string &password)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            long id;
                select << "SELECT id FROM User where login=? and password=?",
                    into(id),
                    use(login),
                    use(password),
                    range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return id;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
        }
        return {};
    }
    std::optional<User> User::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            User a;
            
            select << "SELECT id, first_name, last_name, email, role,login,password FROM User where id=?" 
                    << database::Database::sharding_hint(id),
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._role),
                into(a._login),
                into(a._password),
                use(id),
                range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return a;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            
        }
        return {};
    }

    // std::vector<User> User::read_all()
    // {
    //     try
    //     {
    //         Poco::Data::Session session = database::Database::get().create_session();
    //         Statement select(session);
    //         std::vector<User> result;
    //         User a;
    //         select << "SELECT id, first_name, last_name, email, role, login, password FROM User",
    //             into(a._id),
    //             into(a._first_name),
    //             into(a._last_name),
    //             into(a._email),
    //             into(a._role),
    //             into(a._login),
    //             into(a._password),
    //             range(0, 1); //  iterate over result set one row at a time

    //         while (!select.done())
    //         {
    //             if (select.execute())
    //                 result.push_back(a);
    //         }
    //         return result;
    //     }

    //     catch (Poco::Data::MySQL::ConnectionException &e)
    //     {
    //         std::cout << "connection:" << e.what() << std::endl;
    //         throw;
    //     }
    //     catch (Poco::Data::MySQL::StatementException &e)
    //     {

    //         std::cout << "statement:" << e.what() << std::endl;
    //         throw;
    //     }
    // }

    std::vector<User> User::search(std::string first_name, std::string last_name)
    {
        try
        {   Poco::Data::Session session = database::Database::get().create_session();
            std::vector<User> result;
            User a;
            first_name += "%";
            last_name += "%";
            std::string select_us;
            for (auto &hint : database::Database::get_all_hints())
            {
            Statement select(session);
            select << "SELECT id, first_name, last_name, email, role, login, password FROM User where first_name LIKE ? and last_name LIKE ?"
                    << hint,
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._role),
                into(a._login),
                into(a._password),
                use(first_name),
                use(last_name),
                range(0, 1); //  iterate over result set one row at a time

                while (!select.done())
                {
                    if (select.execute())
                        result.push_back(a);
                }
            }
            
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    void User::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);
            Poco::Data::Statement select(session);
            //SELECT SEQUENCE ID
            long in_id;
            select << "SELECT NEXTVAL(ids) id",
                into(in_id),
                range(0, 1); //  iterate over result set one row at a time
            if (!select.done())
            {
                select.execute();
            }

            //MAIN INPUT
            insert << "INSERT INTO User (id, first_name,last_name,email,role,login,password) VALUES(?, ?, ?, ?, ?, ?, ?)"
                << database::Database::sharding_hint(in_id),
                use(in_id),
                use(_first_name),
                use(_last_name),
                use(_email),
                use(_role),
                use(_login),
                use(_password);

            insert.execute();
            
            std::cout << "inserted:" << std::to_string(in_id);
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    const std::string &User::get_login() const
    {
        return _login;
    }

    const std::string &User::get_password() const
    {
        return _password;
    }

    std::string &User::login()
    {
        return _login;
    }

    std::string &User::password()
    {
        return _password;
    }

    long User::get_id() const
    {
        return _id;
    }

    const std::string &User::get_first_name() const
    {
        return _first_name;
    }

    const std::string &User::get_last_name() const
    {
        return _last_name;
    }

    const std::string &User::get_email() const
    {
        return _email;
    }

    const std::string &User::get_role() const
    {
        return _role;
    }

    long &User::id()
    {
        return _id;
    }

    std::string &User::first_name()
    {
        return _first_name;
    }

    std::string &User::last_name()
    {
        return _last_name;
    }

    std::string &User::email()
    {
        return _email;
    }

    std::string &User::role()
    {
        return _role;
    }
}