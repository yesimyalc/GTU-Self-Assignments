using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneChanger : MonoBehaviour
{
    public GameObject sceneButtonsUI;

    public void changeToFirst()
    {
        int currentIndex=SceneManager.GetActiveScene().buildIndex;
        if(currentIndex!=0)
        {
            sceneButtonsUI.SetActive(false);
            SceneManager.LoadScene(0);
        }
    }

    public void changeToSecond()
    {
        int currentIndex=SceneManager.GetActiveScene().buildIndex;
        if(currentIndex!=1)
        {
            sceneButtonsUI.SetActive(false);
            SceneManager.LoadScene(1);
        }
    }

    public void changeToThird()
    {
        int currentIndex=SceneManager.GetActiveScene().buildIndex;
        if(currentIndex!=2)
        {
            sceneButtonsUI.SetActive(false);
            SceneManager.LoadScene(2);
        }
    }

}
