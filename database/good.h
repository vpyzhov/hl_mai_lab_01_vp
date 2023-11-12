#ifndef GOOD_H
#define GOOD_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Good{
        private:
            long _id;
            std::string _creator_id;
            std::string _name;
            std::string _description;
            std::string _price;
            std::string _active;

        public:

            static Good fromJSON(const std::string & str);

            long             get_id() const;
            const std::string &get_creator_id() const;
            const std::string &get_name() const;
            const std::string &get_description() const;
            const std::string &get_price() const;
            const std::string &get_active() const;

            long&        id();
            std::string &creator_id();
            std::string &name();
            std::string &description();
            std::string &price();
            std::string &active();

            static void init();
            static std::optional<Good> read_by_id(long id);
            static std::vector<Good> read_by_name(std::string name);
            void save_to_mysql();
            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif