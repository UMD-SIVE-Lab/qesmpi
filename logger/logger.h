#ifndef __LOGGER_H__
#define __LOGGER_H__ 1
#include "iostream"
#include "tuple"

using namespace std;
namespace sivelab
{
    typedef enum  
    {
        ALL, DEBUG, INFO, ERROR
    } LOG_LEVEL;

    class logger
    {
        private:
            string class_name;
            LOG_LEVEL log_level;

            template <class T, class... Tail>
            void print(T head,  Tail... tail)
            {
                cout << head << " ";
                print(std::forward<Tail>(tail)...);
            }

            void print()
            {
                cout << endl;
            }
        public:
            logger(){}
            logger(LOG_LEVEL log_level_, string class_name_): log_level(log_level_), class_name(class_name_)
            {

            }
            ~logger() {}

            template <class... Args>
            void error(Args... args)
            {
                if (log_level <= LOG_LEVEL::ERROR)
                {
                    print ("ERROR\t", class_name + ":", args ...);
                }
            }

            template <class... Args>
            void info(Args... args)
            {
                if (log_level <= LOG_LEVEL::INFO)
                {
                    print ("INFO\t", class_name + ":", args ...);
                }
            }

            template <class... Args>
            void debug(Args... args)
            {
                if (log_level <= LOG_LEVEL::DEBUG)
                {
                    print ("DEBUG\t", class_name + ":", args ...);
                }
            }

            void set_log_level(LOG_LEVEL log_level_)
            {
                log_level = log_level_;
            }
    };
}
#endif
