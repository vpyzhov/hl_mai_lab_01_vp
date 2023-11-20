#include "cart.h"
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

    void Cart::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Cart` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`user_id` VARCHAR(256) NOT NULL,"
                        << "`good_id` VARCHAR(256) NOT NULL,"
                        << "`qty` VARCHAR(256) NOT NULL,"
                        << "`cost` VARCHAR(256) NOT NULL,"
                        << "PRIMARY KEY (`id`));",
                now;
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

    Poco::JSON::Object::Ptr Cart::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("user_id", _user_id);
        root->set("good_id", _good_id);
        root->set("qty", _qty);
        root->set("cost", _cost);

        return root;
    }

    Cart Cart::fromJSON(const std::string &str)
    {
        Cart cart;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        cart.id() = object->getValue<long>("id");
        cart.user_id() = object->getValue<std::string>("user_id");
        cart.good_id() = object->getValue<std::string>("good_id");
        cart.qty() = object->getValue<std::string>("qty");
        cart.cost() = object->getValue<std::string>("cost");

        return cart;
    }

    std::vector<Cart> Cart::read_by_id(std::string user_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Cart> result;
            Cart a;
            select << "SELECT id, user_id, good_id, qty, cost FROM Cart where user_id=?",
                into(a._id),
                into(a._user_id),
                into(a._good_id),
                into(a._qty),
                into(a._cost),
                use(user_id),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
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

    void Cart::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Cart (user_id, good_id, qty, cost) VALUES(?, ?, ?, ?)",
                use(_user_id),
                use(_good_id),
                use(_qty),
                use(_cost);
            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
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

    long &Cart::id()
    {
        return _id;
    }
     std::string &Cart::user_id()
    {
        return _user_id;
    }
        std::string &Cart::good_id()
    {
        return _good_id;
    }
         std::string &Cart::qty()
    {
        return _qty;
    }
        std::string &Cart::cost()
    {
        return _cost;
    }

    long Cart::get_id() const
    {
        return _id;
    }
    const std::string &Cart::get_user_id() const
    {
        return _user_id;
    }
    const std::string &Cart::get_good_id() const
    {
        return _good_id;
    }
    const std::string &Cart::get_qty() const
    {
        return _qty;
    }
    const std::string &Cart::get_cost() const
    {
        return _cost;
    }

}