// Init Container
var bgContainer = new UIContainer("container", 0, 1920, 1080);
bgContainer.AddToUIObject();    // 添加容器到渲染线程
bgContainer.SetBlendMode("Screen");   // Alpha Blend

// Background Image
var bgImage = new UIImage("bg", "Resources\\Test\\Texture\\start_scene\\title#bg.png", 0);
bgContainer.AddComponent(bgImage);
bgImage.SetColor(new Vector4(1, 1, 1, 0));

var logoImage = new UIImage("logo", "Resources\\Test\\Texture\\start_scene\\title#logo.png", 26);
bgContainer.AddComponent(logoImage);
logoImage.SetAnchor("TopLeft");
logoImage.SetPosition(new Vector2(50, 50));
logoImage.SetColor(new Vector4(1, 1, 1, 0));

var chbg = new UIImage("chbg", "Resources\\Test\\Texture\\start_scene\\title#title_charall.png", 15);
bgContainer.AddComponent(chbg);
chbg.SetColor(new Vector4(1, 1, 1, 0));

var albg = new UIImage("title#head", "Resources\\Test\\Texture\\start_scene\\title#head.png", 16);
bgContainer.AddComponent(albg);
albg.SetColor(new Vector4(1, 1, 1, 0));
albg.SetAnchor("MiddleLeft");
albg.SetPosition(new Vector2(0, -120));

// Import Characters

var ch1 = new UIImage("ch1", "Resources\\Test\\Texture\\start_scene\\title#ch1_fn.png", 14);
bgContainer.AddComponent(ch1);
ch1.SetAnchor("BottomCenter");
ch1.SetPosition(new Vector2(44.4, 0));
ch1.SetColor(new Vector4(1, 1, 1, 0));

var ch2 = new UIImage("ch2", "Resources\\Test\\Texture\\start_scene\\title#ch2_mrsm.png", 13);
bgContainer.AddComponent(ch2);
ch2.SetAnchor("BottomCenter");
ch2.SetPosition(new Vector2(-263.3, 0));
ch2.SetColor(new Vector4(1, 1, 1, 0));

var ch3 = new UIImage("ch3", "Resources\\Test\\Texture\\start_scene\\title#ch3_mz.png", 12);
bgContainer.AddComponent(ch3);
ch3.SetAnchor("BottomCenter");
ch3.SetPosition(new Vector2(505.8, 0));
ch3.SetColor(new Vector4(1, 1, 1, 0));

var ch4 = new UIImage("ch4", "Resources\\Test\\Texture\\start_scene\\title#ch4_rn.png", 11);
bgContainer.AddComponent(ch4);
ch4.SetAnchor("BottomCenter");
ch4.SetPosition(new Vector2(552.4, 0));
ch4.SetColor(new Vector4(1, 1, 1, 0));

// UIButton Part
var btnContainer = new UIContainer("btnContainer", 20, 550, 687);
bgContainer.AddComponent(btnContainer);
btnContainer.SetAnchor("MiddleLeft");
btnContainer.SetPosition(new Vector2(0, -120));
btnContainer.SetColor(new Vector4(1, 1, 1, 0));

var startPos = 30;

var btn1 = new UIButton("btnStart", 2);
btnContainer.AddComponent(btn1);
btnContainer.SetEnabled(false);
btn1.SetPosition(new Vector2(80, startPos));
btn1.SetAnchor("TopLeft");
btn1.SetNormalTexture("Resources\\Test\\Texture\\start_scene\\2052.png");
btn1.SetHoverTexture("Resources\\Test\\Texture\\start_scene\\2285.png");
btn1.SetPressedTexture("Resources\\Test\\Texture\\start_scene\\2059.png");

startPos += btn1.GetSize().GetY() + 50;
var btn2 = new UIButton("btnLoad", 2);
btnContainer.AddComponent(btn2);
btn2.SetPosition(new Vector2(80, startPos));
btn2.SetAnchor("TopLeft");
btn2.SetNormalTexture("Resources\\Test\\Texture\\start_scene\\2073.png");
btn2.SetHoverTexture("Resources\\Test\\Texture\\start_scene\\2282.png");
btn2.SetPressedTexture("Resources\\Test\\Texture\\start_scene\\2066.png");
// Set Animation

var timeline = new Timeline();
timeline.Activate();    // 添加时间轴到渲染线程

timeline.SetOnFinished("btnContainer.SetEnabled(true);");

function FadeIn(_timeline, _target, _duration, _startTime) {
    _timeline.AddClip(_startTime, new ColorClip(1, _target,
        new Vector4(1, 1, 1, 0),
        new Vector4(1, 1, 1, 1),
        new AnimationCurve("Linear")));
}

function FadeOut(_timeline, _target, _duration, _startTime) {
    _timeline.AddClip(_startTime, new ColorClip(_duration, _target,
        new Vector4(1, 1, 1, 1),
        new Vector4(1, 1, 1, 0),
        new AnimationCurve("Linear")));
}

function Move(_timeline, _target, _duration, _startTime, _startPos, _endPos) {
    _timeline.AddClip(_startTime, new PositionClip(_duration, _target,
        _startPos,
        _endPos,
        new AnimationCurve("Linear")));
}

var bgmClip = new AudioClip("Resources\\Test\\Audio\\opening.wav");
timeline.AddClip(0, bgmClip);

FadeIn(timeline, bgImage, 0.5, 0);
FadeIn(timeline, ch1, .5, 0.5);
FadeIn(timeline, ch2, .5, 0.8);
FadeIn(timeline, ch3, .5, 1.0);
FadeIn(timeline, ch4, .5, 1.3);
FadeIn(timeline, logoImage, 1, 0.5);
FadeIn(timeline, albg, .3, 1.8);
FadeIn(timeline, chbg, .3, 1.8);
FadeIn(timeline, btnContainer, .3, 2.0);

Move(timeline, ch1, .5, 0.5, new Vector2(44.4, -50), new Vector2(44.4, 0));
Move(timeline, ch2, .5, 0.8, new Vector2(-200, 0), new Vector2(-263.3, 0));
Move(timeline, ch3, .5, 1.0, new Vector2(405, 0), new Vector2(505.8, 0));
Move(timeline, ch4, .5, 1.3, new Vector2(450, 0), new Vector2(552.4, 0));

// timeline.SetOnStarted("Print(\"Start Scene Timeline is Playing!\");");
// timeline.SetOnFinished("Print(\"Start Scene Timeline has played to the end!\");");
SetOnGameStart('timeline.Play();'); // 在游戏开始时播放时间轴

// Button Event
var stTimeline = new Timeline();
stTimeline.Activate();
stTimeline.AddClip(0, new ColorClip(0.5, bgContainer, new Vector4(1, 1, 1, 1), new Vector4(0, 0, 0, 1), new AnimationCurve("Linear")));
stTimeline.SetOnFinished("ExecuteFile(\"Scripts/main_scene.js\"); DestroyComponent(bgContainer); ");

btn1.SetOnClick("stTimeline.Play();");
