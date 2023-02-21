using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DontDestroy1 : MonoBehaviour
{
    private static DontDestroy1 playerInstance;
     void Awake()
     {
         DontDestroyOnLoad (this);
         
         if (playerInstance == null) 
             playerInstance = this;
          else 
             Destroy(this.gameObject);
       
     }
}
