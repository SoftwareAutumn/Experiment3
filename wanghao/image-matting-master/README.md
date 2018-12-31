IMAGE MATTING
==
>>本代码的思路是先生成一个黑色背景的superman————BlackBackgroud.jpg，然后将space.jpg的像素加到BlackBackgroud.jpg上，如果这幅图某些点的像素值为全0（黑色部分），就使用space.jpg的像素，否则还使用BlackBackground的像素。做完后才想直接把原图的绿幕用space.jpg的像素代替就好，但是不想改了。还有就是代码略显繁琐，用指针数组就可以写循环的，可以更简练，但是也不想改了，这些代码在滤波器实验里再优化，现在实现了功能就好了。
