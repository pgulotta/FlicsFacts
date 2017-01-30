package com.twentysixapps.flicsfacts;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.text.Html;

public class MovieShareIntent extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static MovieShareIntent m_instance;

    public MovieShareIntent()
    {
        m_instance = this;
    }

   static public void startShareActivity(String content, org.qtproject.qt5.android.bindings.QtActivity activity)
   {
        Intent intent = new Intent(Intent.ACTION_SEND);
        intent.setType("text/plain");
        intent.putExtra(Intent.EXTRA_SUBJECT,  activity.getString(R.string.from_app));
        intent.putExtra(Intent.EXTRA_TEXT, content);
        activity.startActivity(Intent.createChooser(intent, activity.getString(R.string.share_app)));
    }

}


