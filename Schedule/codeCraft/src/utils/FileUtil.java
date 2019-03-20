package utils;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class FileUtil {
    static public FileReader getFileReader(String path) {
        FileReader fileReader = null;
        try {
            fileReader = new FileReader(path);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        return fileReader;
    }
    static  public void closeFileReader(FileReader fileReader) {
        try {
            fileReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    static public BufferedReader getBufferedReader(FileReader fileReader) {
        BufferedReader bufferedReader = null;
        bufferedReader = new BufferedReader(fileReader);
        return bufferedReader;
    }
    static  public void closeBufferedReader(BufferedReader bufferedReader) {
        try {
            bufferedReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
