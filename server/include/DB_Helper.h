#pragma once

#ifndef DB_HELPER_H
#define DB_HELPER_H

#include <memory>

class DB_Helper
{
public:
	std::shared_ptr<DB_Helper> get_Instance();
private:
	DB_Helper();
	virtual ~DB_Helper();
};

#endif