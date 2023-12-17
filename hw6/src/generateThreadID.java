public class generateThreadID {
    public String multiThreaded() {
        int lowerBound = 10000;
        int upperBound = 99999;
        // Generate random int value from min to max
        int randomInt = (int)Math.floor(Math.random() * (upperBound - lowerBound + 1) + lowerBound);
        return "ThreadID-" + randomInt;
    }
}
