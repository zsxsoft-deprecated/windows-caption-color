windows-caption-color
===================================

It's a node extension to get the caption color of an active windows in Windows 8+.

## Example
```javascript
    require('windows-title-color').get(function(err, ret) {
        if (!err) {
            console.log(ret);
        }
    });
```
Will return: 
```javascript
{
    "dwm": {
        "hsv": "c73eb8d0",
        "opaque": true,
        "a": 0.78,
        "r": 62,
        "g": 184,
        "b": 208
    },
    "reg": {
        "hsv": "af4adbf8",
        "balance": 4,
        "a": 0.69,
        "r": 74,
        "g": 219,
        "b": 248
    }
}
```

``dwm`` is from ``DwmGetColorizationColor`` API.

``reg`` is from ``HKCU\Software\Microsoft\Windows\DWM``.

However, the result from ``GetSysColor(COLOR_ACTIVECAPTION)`` is wrong.
## How to use?

In Windows 10, the following code can set your ``document.body``'s background color same as the caption color.
```javascript
    require('windows-title-color').get(function(err, ret) {
        if (!err) {
            window.document.body.style.background = "rgba(" + ret.reg.r + ", " + ret.reg.g + ", " + ret.reg.b + ", " + ret.reg.a + ")";
        }
    });
```

In Windows 8, it will lighter or darker. So if you want to use it directly, you will get a wrong result. And you have to refer to the following article.

http://stackoverflow.com/questions/24555827/how-to-get-title-bar-color-of-wpf-window-in-windows-8-1

http://stackoverflow.com/questions/3560890/vista-7-how-to-get-glass-color

## License
The MIT License