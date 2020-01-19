public class Vec2fNew
{
    private float _x;
    private float _y;

    public Vec2fNew(float x, float y)
    {
	_x = x;
	_y = y;
    }

    public static Vec2fNew new_vec(float x, float y)
    {
	return new Vec2fNew(x, y);
    }

}
