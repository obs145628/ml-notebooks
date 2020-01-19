public final class MyVec
{

    private int _data[];
    private int _size;

    MyVec()
    {
	_data = new int[8];
	_size = 0;
    }

    public int getSize()
    {
	return _size;
    }

    public int getCapacity()
    {
	return _data.length;
    }

    public int getVal(int id)
    {
	return _data[id];
    }

    public void setVal(int id, int val)
    {
	_data[id] = val;
    }

    public void push(int val)
    {
	reserve(_size + 1);
	_data[_size] = val;
	++_size;
    }

    public void reserve(int new_cap)
    {
	int next_cap = getCapacity();
	while (new_cap > next_cap)
	    next_cap *= 2;
	if (next_cap == getCapacity())
	    return;
	
	int[] new_data = new int[next_cap];
	for (int i = 0; i < getSize(); ++i)
	    new_data[i] = _data[i];
	_data = new_data;
    }

}
