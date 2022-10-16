static class stat1

    method void Test2()

        printf("Test2")

    end 

    method void Test3(float a,float b)

        printf("t3:",a,b)

    end 

    method void test4(check m)
    
        printf("Test4",m.val)

    end 

end 

class check

    float val = 25

    method void check()

        printf("Created check",val)

    end 

    method void checkpars(int a,float b)

        printf("cp:",a,b)

    end 

end 

class testClass

    method void testClass()

       #test statement
        printf("Testing")

        #test bare vars
        int a 

        #test assign
        a = 25

        #test vars with default value
        int b = 30

        #test value is correct
        printf("B:",b)

        #test class vars with new
        check m = new check()

        #test class member assign
        m.val = 45
        
        #test class statement
        m.check()

        #test static class statement
        stat1.Test2()

        #test pars
        m.checkpars(20,40)

        #test static pars
        stat1.Test3(30,60)

        #test state return
        a = getVal()
        
        printf("A:",a)

        #test class pars
        checkClass(m)

        #test class return
        m = createM()

        #check return class values
        printf("mval:",m.val)

        #test class pars
        stat1.test4(m)

        #test if/else

        a = -20

        if a<0

            printf("A<0")

        else

            printf("A>0")

        end 

    end 

    method check createM()

        printf("Creating check createM")

        check mm = new check()
        mm.val = 90
        return mm


    end 

    method void checkClass(check pp)
    
        printf("PP",pp.val)
    
    end

    method float getVal()

        return 35

    end 

end 

