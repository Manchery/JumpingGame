#ifndef __DROP_LAND_H__
#define __DROP_LAND_H__

#include "cocos2d.h"

class DropLand : public cocos2d::Sprite
{
public:
	static DropLand* create(const std::string& filename);

	virtual bool initWithFile(const std::string& filename) override;
	void remove();
	void drop();
};

#endif // __DROP_LAND_H__
