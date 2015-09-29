template<bool>
class CompileTimeAssert;

template<>
class CompileTimeAssert<true> {};

int main()
{
	CompileTimeAssert<1 != 0>();
	CompileTimeAssert<1 != 1>();

	return 0;
}
