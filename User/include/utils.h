#ifndef __UTILS_H__
#define __UTILS_H__

#ifndef __INLINE__
#define __INLINE__ inline
#endif

static __INLINE__ void __delay_x(unsigned long ns)
{
	volatile int i = ns;
	while (i--)
		;
}

static __INLINE__ void __delay_us(unsigned long us)
{
	volatile int i;
	while (--us)
	{
		i++;
		i++;
		i++;
	}
}

static __INLINE__ void __delay_ms(unsigned long ms)
{
	volatile int i;
	while (--ms)
	{
		for (i = 0; i < 1500; i++)
			;
	}
}

static __INLINE__ unsigned long a2dec(signed char *str)
{
	unsigned long ret = 0;
	while (*str)
	{
		if ((*str >= '0') && (*str <= '9'))
			ret = (ret * 10) + (*str - '0');
		str++;
	}
	return ret;
}

static __INLINE__ unsigned long a2hex(signed char *str)
{
	unsigned long ret = 0;
	while (*str)
	{
		if ((*str >= '0') && (*str <= '9'))
			ret = (ret << 4) + (*str - '0');
		else if ((*str >= 'a') && (*str <= 'f'))
			ret = (ret << 4) + (*str - 'a' + 10);
		else if ((*str >= 'A') && (*str <= 'F'))
			ret = (ret << 4) + (*str - 'A' + 10);
		str++;
	}
	return ret;
}

static __INLINE__ uint16_t swift(uint8_t *out, uint32_t in, uint8_t bit)
{
	uint8_t *p = out;
    uint16_t cnt = 0;
	uint32_t tmp = in;
	uint32_t swift[10] = {0};
	int i;
	for(i = 0; i < bit; i++)
	{
		swift[bit - 1 - i] = (tmp % 10) + '0';
		tmp = (tmp / 10);
	}
	for(i = 0; i < bit; i++)
	{
		*p++ = swift[i];
        cnt++;
	}
	p = 0;
    return cnt;
}

#define countof(a) (sizeof(a) / sizeof(*(a)))
#endif
