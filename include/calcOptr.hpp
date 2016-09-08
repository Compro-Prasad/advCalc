#ifndef calcOPTR
#define calcOPTR

#include <calcStack.hpp>

typedef unsigned long long str_hash;
typedef unsigned int optr_hash;

class Operator
{
	optr_hash op;
	bool isBinary;
	unsigned char priority;
	bool setOperatorProperties();
public:
	Operator();
	Operator(optr_hash);
	Operator(const char *);
	Operator(const Operator &);
	bool isUnary();
	const char *toString();
	unsigned char setFromString(const char *);
	int priorityGroup();
	Operator operator=(const Operator &);
	bool operator==(const Operator);
	bool operator<=(const Operator);
	bool operator>=(const Operator);
	bool operator<(const Operator);
	bool operator>(const Operator);
	enum
	{
		H_plus            = 43,
		H_minus           = 45,
		H_multiply        = 42,
		H_divide          = 47,
		H_pow             = 94,
		H_mod             = 37,
		H_P               = 80,
		H_C               = 67,
		H_bit_and         = 38,
		H_bit_or          = 124,
		H_bit_shift_right = 7936,
		H_bit_shift_left  = 7680,
		H_bit_not         = 126,
		H_open_bracket    = 40,
		H_close_bracket   = 41,
		H_not             = 33,
		H_and             = 4864,
		H_or              = 15872,
		H_great           = 62,
		H_less            = 60,
		H_great_equal     = 7935,
		H_less_equal      = 7681,
		H_equal           = 7808,
		H_not_equal       = 4252,
		H_sin             = 1868280,
		H_cos             = 1610983,
		H_tan             = 1883393,
		H_sec             = 1867761,
		H_cosec           = 983557758,
		H_cot             = 1610984,
		H_asin            = 200561431,
		H_acos            = 200304134,
		H_atan            = 200576544,
		H_asec            = 200560912,
		H_acosec          = 705393442,
		H_acot            = 200304135,
		H_sinh            = 237271664,
		H_cosh            = 204594945,
		H_tanh            = 239191015,
		H_log             = 1756132,
		H_ln              = 13826,
		H_logten          = 232822054,
		H_abs             = 1577074,
		H_floor           = 757783302,
		H_ceil            = 204432389
	};
};

class operatorManager
{
	calcStack<Operator> operatorStack;
	calcStack<long double> numberStack;
public:
	operatorManager();
};

#endif
