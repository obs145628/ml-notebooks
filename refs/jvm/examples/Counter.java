public class Counter
{

    private static int _instances;

    Counter()
    {
	++_instances;
    }

    public static int get_instances() { return _instances; }

}
