#include <Geode/Geode.hpp>
#include <capeling.garage-stats-menu/include/StatsDisplayAPI.h>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>

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
		
		if (Mod::get()->getSettingValue<bool>("show-in-garage")) {
			checkAndResetDailyStars();

			auto statMenu = this->getChildByID("capeling.garage-stats-menu/stats-menu");

			auto myStatItem = StatsDisplayAPI::getNewItem("daily-stars"_spr, CCSprite::create("Daily_Stars.png"_spr), dailyStars, 0.8f);

			if (statMenu) {
				statMenu->addChild(myStatItem);
				statMenu->updateLayout();
			}

			return true;
		}
		else {
			return true;
		}

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

class $modify(LevelBrowserLayer) {
	$override
	void onEnter() {
		LevelBrowserLayer::onEnter();

		if (Mod::get()->getSettingValue<bool>("show-in-browser")) {
			checkAndResetDailyStars();

			auto menu = this->getChildByID("page-menu");
			auto oldStat = menu->getChildByID("daily-stars");
	
			if (auto oldStat = menu->getChildByID("daily-stars")) {
				oldStat->removeFromParent();
			}
			auto myStatItem = StatsDisplayAPI::getNewItem("daily-stars"_spr, CCSprite::create("Daily_Stars.png"_spr), dailyStars, 0.9f);

			myStatItem->setPosition({ -13, 97 });

			myStatItem->setID("daily-stars");
			if (menu->getChildByID("folder-button") && Loader::get()->isModLoaded("cvolton.betterinfo")) {
				myStatItem->setPosition({ 21, 62 });
			}
			menu->addChild(myStatItem);
			
			if (Mod::get()->getSettingValue<bool>("enable-particles")) {
				CCParticleSystemQuad* starParticles = GameToolbox::particleFromString("30a-1a2a0a8a180a180a0a0a25a50a0a5a-8a0a0a10a5a0a0a0a1a0a1a0a0.25a0a1a0.05a0a0a0a0a1a0a1a0a1a0a0a0a0a0a0.35a0a0a0a20a0a0a0a1a2a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
				starParticles->setZOrder(-1);
				starParticles->setPosition({ 0, 0 });
				myStatItem->addChild(starParticles);
			}

		}
	}
};
