#ifndef CART_H
#define CART_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Cart{
        private:
            long _id;
            std::string _user_id;
            std::string _good_id;
            std::string _qty;
            std::string _cost;

        public:

            static Cart fromJSON(const std::string & str);

            long             get_id() const;
            const std::string &get_user_id() const;
            const std::string &get_good_id() const;
            const std::string &get_qty() const;
            const std::string &get_cost() const;

            long&        id();
            std::string &user_id();
            std::string &good_id();
            std::string &qty();
            std::string &cost();

            static void init();
            static std::vector<Cart> read_by_id(std::string user_id);
            void save_to_mysql();
            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif