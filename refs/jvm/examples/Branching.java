public class Branching
{

    public static void test_if(int p)
    {
	int x;
	if (p > 3)
	    x = 5;
    }

    public static void test_if_else(int p)
    {
	int x;
	if (p == 3)
	    x = 5;
	else
	    x = 7;
    }

    public static void test_while(int[] data)
    {
	if (data == null)
	    return;
	int i = 0;
	while (i < 4)
	{
	    i += 5;
	    data[0] = 6;
	}
    }

}
