public class Fact
{

    public static int get(int x)
    {
	if (x <= 1)
	    return 1;
	else
	    return x * Fact.get(x - 1);
    }

}
