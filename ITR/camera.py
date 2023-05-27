# Import OpenCV
import cv2

# Set up camera object called cap which we will use to find OpenCV
cap = cv2.VideoCapture(0)

# QR code detection Method
detector = cv2.QRCodeDetector()

# This creates an Infinite loop to keep your camera searching for data at all times
while True:
    
    # Below is the method to get a image of the QR code
    _, img = cap.read()
    
    # Below is the method to read the QR code by detecting the bounding box coords and decoding the hidden QR data 
    data, bbox, _ = detector.detectAndDecode(img)
    
    # This is how we get that Blue Box around our Data. This will draw one, and then Write the Data along with the top
    if(bbox is not None):
        for i in range(len(bbox)):
            cv2.line(img, tuple(bbox[i][0]), tuple(bbox[(i+1) % len(bbox)][0]), color=(255, 0, 0), thickness=2)
        cv2.putText(img, data, (int(bbox[0][0][0]), int(bbox[0][0][1]) - 10), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 250, 120), 2)
        
        # Below prints the found data to the below terminal
        if data:
            if data == 'product 1':
                print("Output: 1")
            elif data == 'product 2':
                print("Output: 2")
            elif data == 'product 3':
                print("Output: 3")
            else:
                print("Unknown QR code detected")
        
    # Below will display the live camera feed to the Desktop on Raspberry Pi OS preview
    cv2.imshow("code detector", img)
    
    # At any point if you want to stop the Code all you need to do is press 'q' on your keyboard
    if(cv2.waitKey(1) == ord("q")):
        break
    
# When the code is stopped the below closes all the applications/windows that the above has created
cap.release()
cv2.destroyAllWindows()
