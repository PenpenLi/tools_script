#include "MyHelper.h"

std::string MyHelper::changeToScoreStringRight(int score)
{
	bool _isHaveNum = false;
	std::string _scoreStr = "";
	int _remain = score;
	if(_remain/1000000)
	{
		_scoreStr += StringUtils::format("%d,",_remain/1000000);
		_isHaveNum = true;
	}
	else
		_scoreStr += "**";

	_remain = _remain%1000000;

	if(_remain/100000||_isHaveNum)
	{
		_scoreStr += StringUtils::format("%d",_remain/100000);
		_isHaveNum = true;
	}
	else
		_scoreStr += "*";

	_remain = _remain%100000;

	if(_remain/10000||_isHaveNum)
	{
		_scoreStr += StringUtils::format("%d",_remain/10000);
		_isHaveNum = true;
	}
	else
		_scoreStr += "*";

	_remain = _remain%10000;

	if(_remain/1000||_isHaveNum)
	{
		_scoreStr += StringUtils::format("%d,",_remain/1000);
		_isHaveNum = true;
	}
	else
		_scoreStr += "**";

	_remain = _remain%1000;

	if(_remain/100||_isHaveNum)
	{
		_scoreStr += StringUtils::format("%d",_remain/100);
		_isHaveNum = true;
	}
	else
		_scoreStr += "*";

	_remain = _remain%100;

	if(_remain/10||_isHaveNum)
		_scoreStr += StringUtils::format("%d",_remain/10);
	else
		_scoreStr += "*";

	_remain = _remain%10;

	_scoreStr += StringUtils::format("%d",_remain);

	return _scoreStr;
}

std::string MyHelper::changeToScoreStringCenter(int score)
{
	bool _isHaveNum = false;
	std::string _scoreStr = "";
	int _remain = score;
	if(_remain/1000000)
	{
		_scoreStr += StringUtils::format("%d,",_remain/1000000);
		_isHaveNum = true;
	}

	_remain = _remain%1000000;

	if(_remain/100000||_isHaveNum)
	{
		_scoreStr += StringUtils::format("%d",_remain/100000);
		_isHaveNum = true;
	}

	_remain = _remain%100000;

	if(_remain/10000||_isHaveNum)
	{
		_scoreStr += StringUtils::format("%d",_remain/10000);
		_isHaveNum = true;
	}

	_remain = _remain%10000;

	if(_remain/1000||_isHaveNum)
	{
		_scoreStr += StringUtils::format("%d,",_remain/1000);
		_isHaveNum = true;
	}

	_remain = _remain%1000;

	if(_remain/100||_isHaveNum)
	{
		_scoreStr += StringUtils::format("%d",_remain/100);
		_isHaveNum = true;
	}

	_remain = _remain%100;

	if(_remain/10||_isHaveNum)
		_scoreStr += StringUtils::format("%d",_remain/10);

	_remain = _remain%10;

	_scoreStr += StringUtils::format("%d",_remain);

	return _scoreStr;
}