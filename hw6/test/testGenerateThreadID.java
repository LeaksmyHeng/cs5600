public class testGenerateThreadID {
    public static void main(String[] args){
        generateThreadID gen = new generateThreadID();
        String test1 = gen.multiThreaded();
        String test2 = gen.multiThreaded();
        String test3 = gen.multiThreaded();
        String test4 = gen.multiThreaded();
        String test5 = gen.multiThreaded();
        String test6 = gen.multiThreaded();
        String test7 = gen.multiThreaded();
        String test8 = gen.multiThreaded();
        String test9 = gen.multiThreaded();
        System.out.println(test1);
        System.out.println(test2);
        System.out.println(test3);
        System.out.println(test4);
        System.out.println(test5);
        System.out.println(test6);
        System.out.println(test7);
        System.out.println(test8);
        System.out.println(test9);
    }
}
