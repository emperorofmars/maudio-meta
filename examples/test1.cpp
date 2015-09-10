/*
 * Project Maudio
 * Copyright (C) 2015 Martin Schwarz
 * See LICENSE.txt for the full license
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <memory>
#include "maudio/scene/Scene.hpp"
#include "maudio/scene/TypeManager.hpp"
#include "maudio/scene/Project.hpp"
#include "maudio/MaudioInfo.hpp"
#include "maudio/MaudioPaths.hpp"
#include "maudio/util/String.hpp"

using namespace maudio;

int main(int argc, char *argv[]){
	std::cerr << "test" << std::endl;
	
	std::cerr << "version info: " << getGitCommit() << std::endl;
	std::cerr << "version info: " << getGitBranch() << std::endl;
	std::cerr << "version info: " << getGitDescription() << std::endl;
	std::cerr << "version info: " << getVersionString() << std::endl;
	std::cerr << "version info: " << getVersionMajor() << std::endl;
	std::cerr << "version info: " << getVersionMinor() << std::endl;
	std::cerr << "version info: " << getVersionPatch() << std::endl;
	
	
	std::cerr << "create project" << std::endl;
	
	Project proj1("test_project");
	proj1.setSaveFile("testfile.maup");
	
	std::shared_ptr<Scene> scene1(new Scene("test_scene"));
	proj1.addScene(scene1);
	
	std::cerr << "create nodes" << std::endl;
	
	long sin1 = scene1->add(TypeManager::create("SinusGenerator", "sin1"));
	long read = scene1->add(TypeManager::create("FileReader", "reader"));
	long mix = scene1->add(TypeManager::create("Mixer", "mix"));
	long play = scene1->add(TypeManager::create("Player", "play"));
	long write = scene1->add(TypeManager::create("FileWriter", "writer"));
	
	auto sin1Prop = scene1->get(sin1)->getProperties();
	sin1Prop->getProperty("Channels")->set("2");
	auto sin1Freq = sin1Prop->getKeyableProperty("Frequency");
	sin1Freq->setKey("500", 0);
	sin1Freq->addKey("500", 0.2);
	sin1Freq->addKey("700", 0.200001);
	sin1Freq->addKey("700", 0.7);
	sin1Freq->addKey("500", 0.700001);
	
	sin1Freq->addKey("500", 1.4);
	sin1Freq->addKey("800", 1.400001);
	sin1Freq->addKey("800", 1.9);
	sin1Freq->addKey("500", 1.900001);
	
	auto readCtrl1 = scene1->get(read)->getControl();
	if(!readCtrl1) throw MaudioException("FUUU READCTRL");
	
	std::cerr << "set filename" << std::endl;
	
	try{
		String ret(readCtrl1->callFunction("setFileName", "horde.ogg"));
		if(ret) std::cerr << ret << std::endl;
	}
	catch(std::exception &e){
		std::cerr << "play error: " << e.what() << std::endl;
	}
	
	std::cerr << "connect" << std::endl;
	
	scene1->connect(read, mix);
	scene1->connect(sin1, mix);
	scene1->connect(mix, play);
	scene1->connect(mix, write);
	
	auto playCtrl1 = scene1->get(play)->getControl();
	if(!playCtrl1) throw MaudioException("FUUU PLAYCTRL");
	
	std::cerr << "play" << std::endl;
	
	try{
		playCtrl1->callFunction("play");
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		playCtrl1->callFunction("stop");
	}
	catch(std::exception &e){
		std::cerr << "play error: " << e.what() << std::endl;
	}
	
	auto writeCtrl1 = scene1->get(write)->getControl();
	if(!writeCtrl1) throw MaudioException("FUUU WRITER");
	
	std::cerr << "write" << std::endl;
	
	try{
		String writerret;
		writerret = writeCtrl1->callFunction("setFileName", "test.ogg");
		if(writerret) std::cerr << "writer 01: " << writerret << std::endl;
		writerret = writeCtrl1->callFunction("write", "0;200000");
		if(writerret) std::cerr << "writer 01: " << writerret << std::endl;
		writeCtrl1->stop();
		writerret = writeCtrl1->callFunction("getResult");
		if(writerret) std::cerr << "writer 01: " << writerret << std::endl;
	}
	catch(std::exception &e){
		std::cerr << "play error: " << e.what() << std::endl;
	}
	
	std::cerr << "serialize" << std::endl;
	
	proj1.save();
	
	
	std::cerr << "deserialize" << std::endl;
	
	Project proj2;
	proj2.load("testfile.maup");
	proj2.setSaveFile("testfile2.maup");
	proj2.setName("proj_2");
	
	proj2.getScene((unsigned int)0)->setName("scene_02");
	
	auto playCtrl2 = proj2.getScene((unsigned int)0)->getEnd(0)->getControl();
	if(!playCtrl2) throw MaudioException("FUUU 02");
	
	std::cerr << "play" << std::endl;
	
	try{
		playCtrl2->callFunction("play");
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		playCtrl2->callFunction("stop");
	}
	catch(std::exception &e){
		std::cerr << "play error: " << e.what() << std::endl;
	}
	
	proj2.save();
	
	std::cerr << "closing main" << std::endl;
	return 0;
}



















