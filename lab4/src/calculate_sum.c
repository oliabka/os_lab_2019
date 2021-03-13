int CalcSum(int *array, unsigned int begin, unsigned int end) {
    int sum=0;
    int i;
    for (i=begin; i<end;i++)
    {
        sum+=*(array+i);
    }
  return sum;
}