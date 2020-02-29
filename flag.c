int			flag_is_set(char flags, char which)
{
	return ((flags & which) != 0);
}
