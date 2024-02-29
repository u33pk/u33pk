subject='/C=US/ST=California/L=Mountain View/O=Android/OU=Android/CN=Android/emailAddress=android@android.com'
mkdir ~/.android-certs
for x in releasekey platform shared media networkstack; do \
    ./development/tools/make_key ~/.android-certs/$x "$subject"; \
done