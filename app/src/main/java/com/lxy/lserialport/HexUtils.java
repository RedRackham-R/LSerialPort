package com.lxy.lserialport;


public class HexUtils {


    /**
     * byte数值转int
     * @param src
     * @return
     */
    public static int byteToInt(byte src){
        return src & 0xff;
    }

    /**
     *Convert byte[] to hex string.这里我们可以将byte转换成int，然后利用Integer.toHexString(int)来转换成16进制字符串。
     *@param src byte[] data
     *@return hex string
     */
    public static String bytesToHexString(byte[] src) {
        StringBuilder stringBuilder = new StringBuilder();
        if (src == null || src.length <= 0) {
            return null;
        }
        for (byte aSrc : src) {
            int v = aSrc & 0xFF;
            String hv = Integer.toHexString(v);
            if (hv.length() < 2) {
                stringBuilder.append(0);
            }
            stringBuilder.append(hv);
        }
        return stringBuilder.toString();
    }

    /**
     * Convert hex string to byte[]
     *
     * @param hexString the hex string
     * @return byte[]
     */
    public static byte[] hexStringToBytes(String hexString) {
        if (hexString == null || hexString.equals("")) {
            return null;
        }
        hexString = hexString.toUpperCase();
        int length = hexString.length() / 2;
        char[] hexChars = hexString.toCharArray();
        byte[] d = new byte[length];
        for (int i = 0; i < length; i++) {
            int pos = i * 2;
            d[i] = (byte) (charToByte(hexChars[pos]) << 4 | charToByte(hexChars[pos + 1]));
        }

      //  System.out.println("hexString = "+hexString+"   ------>"+Arrays.toString(d));
        return d;
    }


    /**
     * Convert char to byte
     *
     * @param c char
     * @return byte
     */
    private static byte charToByte(char c) {
        return (byte) "0123456789ABCDEF".indexOf(c);
    }


    /**
     * Hex字符串转byte
     * @param inHex 待转换的Hex字符串
     * @return  转换后的byte
     */
    public static byte hexToByte(String inHex){
        return (byte)Integer.parseInt(inHex,16);
    }

    /**
     * 字节数组拆分
     *
     * @param paramArrayOfByte 原始数组
     * @param start        起始下标
     * @param offset        要截取的长度
     * @return 处理后的数组
     */
    public static byte[] subArray(byte[] paramArrayOfByte, int start, int offset) {
        byte[] arrayOfByte = new byte[offset];
        int i = 0;
        while (true) {
            if (i >= offset)
                return arrayOfByte;
            arrayOfByte[i] = paramArrayOfByte[(i + start)];
            i += 1;
        }
    }


    //使用1字节就可以表示b 00
    public static String intToHex8(int b) {
        return String.format("%02x", b);//2表示需要两个16进行数
    }
    //需要使用2字节表示b  0000
    public static String intToHex16(int b) {
        return String.format("%04x", b);
    }
    //需要使用4字节表示b 00000000
    public static String intToHex32(int b) {
        return String.format("%08x", b);
    }



    /* c 要填充的字符
     *  length 填充后字符串的总长度
     *  content 要格式化的字符串
     *  格式化字符串，左对齐
     * */
    public static String flushLeft( long length, String content){
        String str;
        StringBuilder cs = new StringBuilder();
        if (content.length() > length){

        }else{
            for (int i = 0; i < length - content.length(); i++){
                cs.append("0");
            }
        }
        str =  cs+content ;
        return str;
    }


    /**
     * 字符串转化成为16进制字符串
     * @param s
     * @return
     */
    public static String stringTo16(String s) {
        String str = "";
        for (int i = 0; i < s.length(); i++) {
            int ch = (int) s.charAt(i);
            String s4 = Integer.toHexString(ch);
            str = str + s4;
        }
        return str;
    }


    /**
     * int到byte[]
     * @param i
     * @return
     */
    public static byte[] intToByteArray(int i) {
        byte[] result = new byte[4];
        //由高位到低位
        result[0] = (byte)((i >> 24) & 0xFF);
        result[1] = (byte)((i >> 16) & 0xFF);
        result[2] = (byte)((i >> 8) & 0xFF);
        result[3] = (byte)(i & 0xFF);
        return result;
    }

    /**
     * byte[]转int
     * @param bytes
     * @return
     */
    public static int byteArrayToInt(byte[] bytes) {
        int value= 0;
        //由高位到低位
        for (int i = 0; i < 4; i++) {
            int shift= (4 - 1 - i) * 8;
            value +=(bytes[i] & 0x000000FF) << shift;//往高位游
        }
        return value;
    }

    public static String toHexString(byte b){
        String s = Integer.toHexString(b & 0xFF);
        if (s.length() == 1){
            return "0" + s;
        }else{
            return s;
        }
    }




}