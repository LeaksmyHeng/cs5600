public class Main {
    public static void main(String[] args) {
        final int capacity = 50;
        final int numberOfProducerThreads = 3;
        final int numberOfConsumerThreads = 3;
        final int numberOfStringPerThread = 30;

        // create and start multiple threads to insert data
        BoundedBuffer buffer = new BoundedBuffer(capacity);
        for (int i = 0; i< numberOfProducerThreads; i++) {
            Thread thread = new Thread(() -> {
                for (int j = 0; j< numberOfStringPerThread; j++) {
                    generateThreadID gen = new generateThreadID();
                    String string = gen.multiThreaded();
                    try {
                        long threadId = Thread.currentThread().getId();
                        System.out.println("ThreadId " + threadId + " Deposit: " + string);
                        buffer.deposit(string);
                    }
                    catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            });
            thread.start();
        }

        // Create and start a thread to fetch data from the buffer
        for (int i=0; i<numberOfConsumerThreads; i++) {
            Thread consumerThread = new Thread(() -> {
                for (int j = 0; j<numberOfProducerThreads* numberOfStringPerThread; j++) {
                    try {
                        long threadId = Thread.currentThread().getId();
                        String string = buffer.fetch();
                        System.out.println("ThreadId " + threadId + " Fetch: " + string);
                    }
                    catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            });
            consumerThread.start();
        }
    }
}