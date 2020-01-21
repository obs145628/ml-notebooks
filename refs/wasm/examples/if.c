

int foo(int x)
{
  int y = x * 2;
  if (x > 10)
  {
    int z = x * 3;
    if (x > z)
      x += 5;
    else if (x < 3)
      x *= 100;
  }
  else {
    x *= y + 67;
  }

  return x + y;
}

int bar(int x, int y)
{
  if (x < 0 || y < 0)
    return x + y;
  else if (x > y)
    return x;
  else if (x < y)
    return y;
  else
    return 100;
}
