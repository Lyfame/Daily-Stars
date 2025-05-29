#include <Geode/Geode.hpp>
#include <capeling.garage-stats-menu/include/StatsDisplayAPI.h>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

using namespace geode::prelude;

int dailyStars = Mod::get()->getSavedValue<int>("dailyStars");

std::string getCurrentDate() {
	auto currentTime = time(0);
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wdeprecated-declarations"
	auto currentTimeinfo = std::localtime(&currentTime);
	#pragma clang diagnostic pop
	std::ostringstream oss;
	oss << std::put_time(currentTimeinfo, "%d-%m-%Y");
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
			
		checkAndResetDailyStars();

		auto statMenu = this->getChildByID("capeling.garage-stats-menu/stats-menu");

		auto myStatItem = StatsDisplayAPI::getNewItem("daily-stars"_spr, CCSprite::create("Daily_Stars.png"_spr), dailyStars, 0.8f);

		if (statMenu) {
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



