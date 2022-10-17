using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class OpenSceneButtons0 : MonoBehaviour
{
    public GameObject sceneButtonsUI;
    
    private void Start()
    {
        sceneButtonsUI.SetActive(false);

        SwipeDetector.OnSwipe += SwipeDetector_OnSwipe;
    }

    private void SwipeDetector_OnSwipe(SwipeData data)
    {
        if (data.Direction == SwipeDirection.Right && sceneButtonsUI.activeSelf==false)
            sceneButtonsUI.SetActive(true);
        else if(data.Direction == SwipeDirection.Left)
            sceneButtonsUI.SetActive(false);
    }

}
