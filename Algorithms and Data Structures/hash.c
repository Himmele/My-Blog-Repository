static unsigned char hash(unsigned int x)
{
	unsigned int hash = 1315423911;
	int i;

	for (i = 0; i < 4; i++) {
		hash ^= ((hash << 5) + (x >> (i*8) & 0xff) + (hash >> 2));
	}

	return hash;
}
