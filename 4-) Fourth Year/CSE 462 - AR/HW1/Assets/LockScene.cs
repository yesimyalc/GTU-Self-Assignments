using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LockScene : MonoBehaviour
{
    private GameObject planeFinder;
    private void Start()
    {
        SwipeDetector.OnSwipe += SwipeDetector_OnSwipe;
    }

    private void SwipeDetector_OnSwipe(SwipeData data)
    {
        if(planeFinder==null)
            planeFinder=GameObject.FindWithTag("PlaneD");
        if (data.Direction == SwipeDirection.Down)
            planeFinder.SetActive(false);
        else if (data.Direction == SwipeDirection.Up)
           planeFinder.SetActive(true);
    }
}
