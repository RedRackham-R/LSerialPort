package com.lxy.lserialport

import android.content.Context
import android.util.AttributeSet
import java.util.*

class LogTextView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0
) : androidx.appcompat.widget.AppCompatTextView(context, attrs, defStyleAttr) {

    private val MAX_LINES = 500 // 最多显示的行数
    private var mLogList: LinkedList<String> = LinkedList<String>()

    override fun setText(text: CharSequence?, type: BufferType?) {
        if (text == null || text.isEmpty()) {
            //用到Viewbinding会比构造函数提前调用setText，这里判断下
            if (mLogList != null) {
                mLogList.clear()
            }
            super.setText(text, type)
            return
        }

        val str = text.toString()
        val lines = str.split("\n").toTypedArray()

        // 超过最大行数时删除最旧的文本
        while (mLogList.size + lines.size > MAX_LINES) {
            mLogList.removeFirst()
        }

        // 添加新的文本
        for (line in lines) {
            mLogList.addLast(line)
        }

        // 构造新的文本内容
        val builder = StringBuilder()
        for (line in mLogList) {
            builder.append(line).append('\n')
        }
        super.setText(builder.toString(), type)
    }

}
