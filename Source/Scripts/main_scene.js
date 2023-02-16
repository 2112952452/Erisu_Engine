var mainBgContainer = new UIContainer("mainBgContainer", 0, 1920, 1080);
mainBgContainer.AddToUIObject();
mainBgContainer.SetBlendMode("Screen");
mainBgContainer.SetColor(new Vector4(0, 0, 0, 1));

var mainBgImage = new UIImage("testBg", "Resources\\Test\\Texture\\main_scene\\QQ图片20221008144043.png", 0);
mainBgContainer.AddComponent(mainBgImage);

var textBoxContainer = new UIContainer("textBoxContainer", 0, 1920, 263);
mainBgContainer.AddComponent(textBoxContainer);
textBoxContainer.SetAnchor("BottomCenter");


var textBoxBg = new UIImage("textBoxBg", "Resources\\Test\\Texture\\main_scene\\11451.png", 0);
textBoxContainer.AddComponent(textBoxBg);
textBoxBg.SetColor(new Vector4(1, 0.5, 0.5, 1));

var text = new UIText("text", "测试字符串", 1);
textBoxContainer.AddComponent(text);
text.SetAnchor("TopLeft");
text.SetPosition(new Vector2(200, 30));


// Load Scene
var mainSceneTimeline = new Timeline();
mainSceneTimeline.Activate();
mainSceneTimeline.AddClip(0, new ColorClip(0.5, mainBgContainer, new Vector4(0, 0, 0, 1), new Vector4(1, 1, 1, 1), new AnimationCurve("Linear")));
mainSceneTimeline.Play();
