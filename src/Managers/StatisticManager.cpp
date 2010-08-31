#include "Managers/StatisticManager.hpp"

StatisticManager::StatisticManager()
{
    using namespace MyGUI;

    mStatsVisible = false;

    CONNECT(Engine::Events::KeyEvent, "KeyPressed", &StatisticManager::keyPressed);
    mStatsWrapper = guiSystem.handle()->createWidget<Widget>("PanelEmpty", 5, 5, 200, 0, 
            Align::Default, "Info", "StatisticWrapper");
    mStatsWrapper->setVisible(mStatsVisible);
}

void StatisticManager::keyPressed(const OIS::KeyEvent &e)
{
    using namespace Engine::Events;

    if (e.key == OIS::KC_I)
    {
        if (inputSystem.getKeyboard()->isModifierDown(OIS::Keyboard::Ctrl))
        {
            mStatsVisible = !mStatsVisible;
            mStatsWrapper->setVisible(mStatsVisible);

            if (mStatsVisible)
            {
                mStatsUpdate = CONNECT0(GlobalUpdateEvent, "Updated", &StatisticManager::update);
            }
            else
            {
                if (mStatsUpdate.connected()) mStatsUpdate.disconnect();
            }
        }
    }
}

void StatisticManager::update()
{
    using namespace MyGUI;

    string pattern = "#FFFFFF %1%: #ffe1bd %2%";

    BOOST_FOREACH(CallbackType::value_type &row, mCallbacks)
    { 
        string result = boost::apply_visitor(call_visitor(), row.second);
        guiSystem.handle()->findWidget<StaticText>(row.first)->setCaption(
                FORMAT(pattern, row.first % result));
    }
}

void StatisticManager::createWidget(const string &name)
{
    using namespace MyGUI;
    int height = mStatsWrapper->getSize().height;

    mStatsWrapper->setSize(mStatsWrapper->getSize().width, height+25);
    mStatsWrapper->createWidget<StaticText>("StaticText", 0, height, 
            200, 25, Align::Default, name)->setFontName("font_Pneumatic.26");
}

void StatisticManager::trackValue(const string &name, const Callback &f)
{
    createWidget(name);
    mCallbacks[name] = f;
}
