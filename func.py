header = "<html><head><title>교수님의 시험지를 훔쳐라!</title></head><body><h1>교수님의 시험지를 훔쳐라!</h1>유능한 KENTECH의 해커 A모씨 일당은 '컴퓨터 구조와 시스템' 수업 교수님의 개인 컴퓨터를 해킹하는데 성공하였습니다. A모씨는 자신의 해킹 능력을 과시하기 위해 여러분도 교수님의 개인 컴퓨터를 해킹할 수 있도록 길을 만들었습니다.<form action=\"/\" method=\"post\"><table><tr><td>Enter the password:</td><td><input name=\'password\' type=\'password\' /></td></tr><tr><td colspan=\"2\" align=\"center\"><button type=\"submit\">제출</button></td></tr>"
inter = "</table></form><h2>정당한 로그인 방법</h2>정당한 패스워드는 correctpassword입니다. 이를 이용하면 잠금이 풀립니다.<br><h2>서버 소스코드</h2>해커 A모씨 일당은 교수님의 개인 컴퓨터에서 탈취한 보안 서버의 취약점을 발견하였습니다. 아래 코드를 보면 우회할 수 있는 방법이 있다고 하네요.<br>아래 변수 buf는 아래 코드와 동일한 함수에 선언되어 있는 배열이며, 여기에 여러분이 입력한 패스워드가 저장됩니다.<br><br>if (!strncmp((const char *)buf, answer, strlen(answer)))<br>{ <br>&nbsp;&nbsp;&nbsp;&nbsp;printf(\"[*] Password is correct!\n\");<br>&nbsp;&nbsp;&nbsp;&nbsp;pass = 1;<br>} <br>   else <br>{ <br>&nbsp;&nbsp;&nbsp;&nbsp;printf(\"[*] Password is incorrect!\n\");<br>} <br><br>if (pass)<br>{<br>&nbsp;&nbsp;&nbsp;&nbsp;printf(\"[*] The client is logged in and has the root privilege!\n\");<br>}<br><h2>서버에서 사용하는 변수들 주소</h2>"
footer = "</body></html>"

def get_address(aname):
    ret = {}

    with open(aname, "r") as f:
        for line in f:
            name, addr = line.strip().split(": ")
            ret[name.strip()] = addr.strip()

    print ("ret: {}".format(ret))
    return ret

def make_index_html(addr):
    ret = header
    ret += inter
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;buf: {}<br>".format(addr["buf"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;len: {}<br>".format(addr["len"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;ret: {}<br>".format(addr["ret"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;pass: {}<br>".format(addr["pass"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;answer: {}<br>".format(addr["answer"])
    ret += footer
    return ret

def make_success_html(addr):
    ret = None

    with open("success.html", "r") as f:
        ret = f.read()

    return ret
    """
    ret = header
    ret += "<tr><td colspan=\"2\" align=\"center\">정당하지 않은 방법으로 로그인에 성공하였습니다! 해킹의 자질이 있군요!</td></tr>"
    ret += inter
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;buf: {}<br>".format(addr["buf"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;len: {}<br>".format(addr["len"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;ret: {}<br>".format(addr["ret"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;pass: {}<br>".format(addr["pass"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;answer: {}<br>".format(addr["answer"])
    ret += footer
    return ret
    """

def make_failure_html(addr):
    ret = header
    ret += "<tr><td colspan=\"2\" align=\"center\">땡! 로그인에 실패하였습니다. 다시 한번 도전해 보시지요!</td></tr>"
    ret += inter
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;buf: {}<br>".format(addr["buf"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;len: {}<br>".format(addr["len"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;ret: {}<br>".format(addr["ret"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;pass: {}<br>".format(addr["pass"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;answer: {}<br>".format(addr["answer"])
    ret += footer
    return ret

def make_valid_html(addr):
    ret = header
    ret += "<tr><td colspan=\"2\" align=\"center\">정당한 패스워드로 로그인을 성공하셨군요. 정당하지 않은 방법으로도 로그인 성공 해보세요!</td></tr>"
    ret += inter
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;buf: {}<br>".format(addr["buf"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;len: {}<br>".format(addr["len"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;ret: {}<br>".format(addr["ret"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;pass: {}<br>".format(addr["pass"])
    ret += "&nbsp;&nbsp;&nbsp;&nbsp;answer: {}<br>".format(addr["answer"])
    ret += footer
    return ret
