#include <Geode/Geode.hpp>
#include <capeling.garage-stats-menu/include/StatsDisplayAPI.h>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

using namespace geode::prelude;

int dailyStars = Mod::get()->getSavedValue<int>("dailyStars");

std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm;
    localtime_s(&local_tm, &now_c);
    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%-%mY-%d");
    return oss.str();
}

void checkAndResetDailyStars() {
    std::string currentDate = getCurrentDate();
    std::string lastDate = Mod::get()->getSavedValue<std::string>("lastDailyStarsReset");

    if (lastDate != currentDate) {
        dailyStars = 0;
        Mod::get()->setSavedValue<int>("dailyStars", 0);
        Mod::get()->setSavedValue<std::string>("lastDailyStarsReset", currentDate);
    }
}

class $modify(GJGarageLayer) {
	bool init() {
		if (!GJGarageLayer::init())
			return false;

		auto statMenu = this->getChildByID("capeling.garage-stats-menu/stats-menu");

		auto myStatItem = StatsDisplayAPI::getNewItem("daily-stars"_spr, CCSprite::create("Daily_Stars.png"_spr), dailyStars, 0.8f);

		if (statMenu) {
			checkAndResetDailyStars();
			statMenu->addChild(myStatItem);
			statMenu->updateLayout();
		}

		return true;
	}
};

class $modify(EndLevelLayer) { 
	void showLayer(bool toggle){
		EndLevelLayer::showLayer(toggle);
		checkAndResetDailyStars();
		dailyStars += m_stars;  
		Mod::get()->setSavedValue<int>("dailyStars", dailyStars);
	}		
};



