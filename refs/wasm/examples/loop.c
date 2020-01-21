
void foo();

void foo_n(int n)
{
  for (int i = 0; i < n; ++i)
    foo();
}

int fact(int x)
{
  int res = 1;
  while (x > 1)
  {
    res *= x;
    x -= 1;
  }
  return res;
}

int el_break(int x)
{

  int res = 0;

  for (int i = x / 3; i < x; ++i)
  {
    if (i % 4 == 0)
      break;
    ++res;
  }

  return res;
}

int el_continue(int x)
{

  int res = 0;

  for (int i = x / 3; i < x; ++i)
  {
    if (i % 4 == 0)
      continue;
    ++res;
  }

  return res;
}
