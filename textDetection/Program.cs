using Google.Apis.Auth.OAuth2;
using Google.Apis.Services;
using Google.Apis.Vision.v1;
using Google.Apis.Vision.v1.Data;
using System;
using System.Collections.Generic;

namespace GoogleCloudVisionTextDetection
{
    public class TextDetection
    {
        /// <summary>
        /// Creates an authorized Cloud Vision client service using Application 
        /// Default Credentials.
        /// </summary>
        /// <returns>an authorized Cloud Vision client.</returns>
        public VisionService CreateAuthorizedClient()
        {
            GoogleCredential credential =
                GoogleCredential.GetApplicationDefaultAsync().Result;
            // Inject the Cloud Vision scopes
            if (credential.IsCreateScopedRequired)
            {
                credential = credential.CreateScoped(new[]
                {
                    VisionService.Scope.CloudPlatform
                });
            }
            return new VisionService(new BaseClientService.Initializer
            {
                HttpClientInitializer = credential,
                GZipEnabled = false
            });
        }

        /// <summary>
        /// Detect texts for an image using the Cloud Vision API.
        /// </summary>
        /// <param name="vision">an authorized Cloud Vision client.</param>
        /// <param name="imagePaths">the path where the images is stored.</param>
        /// <returns>a list of texts detected by the Vision API for the image.
        /// </returns>
        public IList<AnnotateImageResponse> DetectTexts(
            VisionService vision, IList<string> imagePaths)
        {
            IList<AnnotateImageRequest> req = new List<AnnotateImageRequest>();
            foreach (var imagePath in imagePaths)
            {
                // Convert image to Base64 encoded for JSON ASCII text based request   
                byte[] imageArray = System.IO.File.ReadAllBytes(imagePath);
                string imageContent = Convert.ToBase64String(imageArray);

                req.Add(new AnnotateImageRequest()
                {
                    Features = new[] { new Feature() { Type = "TEXT_DETECTION" } },
                    Image = new Image() { Content = imageContent }
                });
            }

            // Post text detection request to the Vision API
            var responses = vision.Images.Annotate(
                new BatchAnnotateImagesRequest()
                {
                    Requests = req
                }
            ).Execute();

            return responses.Responses;
        }

        private static int Main(string[] args)
        {
            //標準入力から画像ファイル名のリストを取得する
            IList<string> imagePaths = new List<string>();
            for (;;)
            {
                string s = Console.ReadLine();
                if (s == null)
                {
                    break;
                }

                imagePaths.Add(s);
            }

            //画像ファイルがひとつもなければ終了
            if(imagePaths.Count < 1)
            {
                Console.WriteLine("No image.");
                return 1;   //エラー
            }

            try
            {
                TextDetection detector = new TextDetection();

                // Create a new Cloud Vision client authorized via Application 
                // Default Credentials
                VisionService vision = detector.CreateAuthorizedClient();

                // Use the client to get texts for the given image
                IList<AnnotateImageResponse> result = detector.DetectTexts(
                    vision, imagePaths);

                // Check if texts were found
                if (result == null)
                {
                    Console.WriteLine("No results.");
                    return 1;   //エラー
                }

                // Loop through and output texts for the image
                var i = 0;
                foreach (var response in result)
                {
                    //Console.WriteLine("Text for image: " + imagePaths[i]);
                    if (response.TextAnnotations != null)
                    {
                        foreach (var text in response.TextAnnotations)
                        {
                            //標準出力に認識結果を出力
                            Console.WriteLine(text.Description);
                            break;  //1行目で抜ける
                        }
                    }
                    i++;
                }

                return 0;   //正常
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return 1;   //エラー
            }
        }
    }
}