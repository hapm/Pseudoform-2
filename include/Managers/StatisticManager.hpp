#ifndef _STATISTIC_MANAGER_HPP_
#define _STATISTIC_MANAGER_HPP_

#include <boost/serialization/singleton.hpp>
#include <boost/function.hpp>
#include <boost/any.hpp>
#include <boost/variant.hpp>

#include "Managers/LogManager.hpp"
#include "Managers/EventManager.hpp"
#include "Systems/InputSystem.hpp"
#include "Systems/GuiSystem.hpp"
#include "Core/Utils.hpp"

#include <map>
#include <utility>

using namespace std;
using namespace boost::serialization;
using namespace boost;

#define statisticManager (StatisticManager::get_mutable_instance())
#define statisticManagerConst (StatisticManager::get_const_instance())

#define TRACK_VALUE_FLOAT(name, fun_ptr, parent) {   \
    function<float ()> arg = bind(fun_ptr, parent);  \
    statisticManager.trackValue(name, arg);          \
}

#define TRACK_VALUE_INT(name, fun_ptr, parent) {     \
    function<int()> arg = bind(fun_ptr, parent);     \
    statisticManager.trackValue(name, arg);          \
}

#define TRACK_VALUE_DOUBLE(name, fun_ptr, parent) {  \
    function<double ()> arg = bind(fun_ptr, parent); \
    statisticManager.trackValue(name, arg);          \
}

#define TRACK_VALUE_STRING(name, fun_ptr, parent) {  \
    function<string ()> arg = bind(fun_ptr, parent); \
    statisticManager.trackValue(name, arg);          \
}


struct call_visitor : public boost::static_visitor<string> {
    template <typename T>
    string operator() (const T& operand) const {
        return lexical_cast<string>(operand());
    }
};

class StatisticManager : public singleton<StatisticManager>
{
    private:
        bool mStatsVisible;

        signals2::connection mStatsUpdate;
        MyGUI::Widget *mStatsWrapper;

        typedef boost::variant<
               boost::function<string()>,
               boost::function<int()>,
               boost::function<float()>,
               boost::function<double()>
        > Callback;
        typedef map<string, Callback> CallbackType;
        CallbackType mCallbacks;

        void createWidget(const string &name);

    public:
        StatisticManager();
        void keyPressed(const OIS::KeyEvent &e);
        void update();

        void trackValue(const string &name, const Callback &f);
};

#endif /* _STATISTIC_MANAGER_HPP_ */
