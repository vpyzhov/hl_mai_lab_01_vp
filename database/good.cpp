#include "good.h"
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

    void Good::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Good` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`creator_id` VARCHAR(256) NOT NULL,"
                        << "`name` VARCHAR(256) NOT NULL,"
                        << "`description` VARCHAR(256) NOT NULL,"
                        << "`price` VARCHAR(256) NOT NULL,"
                        << "`active` VARCHAR(256) NULL,"
                        << "PRIMARY KEY (`id`),KEY `fn` (`first_name`),KEY `ln` (`last_name`));",
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

    Poco::JSON::Object::Ptr Good::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("creator_id", _creator_id);
        root->set("name", _name);
        root->set("description", _description);
        root->set("price", _price);
        root->set("active", _active);

        return root;
    }

    Good Good::fromJSON(const std::string &str)
    {
        Good good;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        good.id() = object->getValue<long>("id");
        good.creator_id() = object->getValue<std::string>("creator_id");
        good.name() = object->getValue<std::string>("name");
        good.description() = object->getValue<std::string>("description");
        good.price() = object->getValue<std::string>("price");
        good.active() = object->getValue<std::string>("active");

        return good;
    }

    std::optional<Good> Good::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            long id;
            Good a;
            select << "SELECT id, creator_id, name, description, price,active FROM Good where id=?",
                into(a._id),
                into(a._creator_id),
                into(a._name),
                into(a._description),
                into(a._price),
                into(a._active),
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


    long &Good::id()
    {
        return _id;
    }
    std::string &Good::creator_id()
    {
        return _creator_id;
    }
        std::string &Good::name()
    {
        return _name;
    }
        std::string &Good::description()
    {
        return _description;
    }
        std::string &Good::price()
    {
        return _price;
    }
        std::string &Good::active()
    {
        return _active;
    }

    long Good::get_id() const
    {
        return _id;
    }
    const std::string &Good::get_creator_id() const
    {
        return _creator_id;
    }
    const std::string &Good::get_name() const
    {
        return _name;
    }
    const std::string &Good::get_description() const
    {
        return _description;
    }
    const std::string &Good::get_price() const
    {
        return _price;
    }
    const std::string &Good::get_active() const
    {
        return _active;
    }
}