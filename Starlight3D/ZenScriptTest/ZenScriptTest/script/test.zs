class testClass


  method void check()
   
    other o1 = new other()

    o1.test1(o1.getVal())

  end    
 
  method int test2()

    return 70

  end 


 

end 

class other

  int ab = 7

  method void other()

    printf("other constructed.")

  end 

  method int test1(int b)

    printf("test1 called",b)

  end 

  method int getVal()

    return 80+ab

  end 

end 