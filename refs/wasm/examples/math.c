
int fact(int x)
{
  return x <= 1 ? 1 : x * fact(x-1);
}

int add(int a, int b)
{
  return a + b;
}

int my_pow(int x, int n)
{
  if (n == 0)
    return 1;
  else if (n == 1)
    return x;
  else if (n == 2)
    return x*x;
  else if (n % 2 == 0)
    return my_pow(x*x, n/2);
  else
    return my_pow(x*x, n/2) * x;
}

int my_abs(int x)
{
  return x <= 0 ? -x : x;
}

int is_null(int x)
{
  return x == 0;
}
