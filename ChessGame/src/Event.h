/* Copyright (c) 2022 Szabadi László Zsolt
 * You should have received a copy of the GNU AGPL v3.0 license with
 * this file. If not, please write to: pliexe, or visit : https://github.com/Pliexe/VisualDiscordBotCreator/blob/master/LICENSE
 */
#pragma once

#include "pch.h"

template <typename Type, typename... Arguments>
class Event
{
private:
	std::vector<std::function<Type(Arguments...)>*> callbacks;
public:
	void operator += (std::function<Type(Arguments...)> callback) {
		this->callbacks.emplace_back(&callback);
	}

	void operator += (std::function<Type(Arguments...)>* callback) {
		this->callbacks.emplace_back(callback);
	}

	/*void operator += (lambda [](Arguments...)->Type* callback) {
		this->callbacks.emplace_back(callback);
	}*/

	void operator -= (std::function<Type(Arguments...)>* callback) {
		this->callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
	}

	/*void operator -= (lambda[](Arguments...)->Type* callback) {
		this->callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
	}*/

	void operator() (Arguments... args) {
		for(auto& callback : callbacks)
			(*callback)(args...);
	}
};