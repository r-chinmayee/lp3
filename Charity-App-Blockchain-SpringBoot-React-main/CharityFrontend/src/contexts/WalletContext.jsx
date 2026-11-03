import { createContext, useContext, useEffect, useState, useRef } from "react";
import { ethers } from "ethers";
import { CONTRACT_ADDRESS, CONTRACT_ABI } from "../config/contract";

const WalletContext = createContext(null);
const EXPECTED_CHAIN_ID = 11155111n; // Sepolia testnet

const createContract = async (provider, signer) => {
  const network = await provider.getNetwork();
  if (network.chainId !== EXPECTED_CHAIN_ID) {
    throw new Error("Switch MetaMask to the Sepolia network to use the charity contract.");
  }

  const code = await provider.getCode(CONTRACT_ADDRESS);
  if (code === "0x") {
    throw new Error("No charity contract is deployed at the configured Sepolia address.");
  }

  return new ethers.Contract(CONTRACT_ADDRESS, CONTRACT_ABI, signer);
};

export const WalletProvider = ({ children }) => {
  const [provider, setProvider] = useState(null);
  const [signer, setSigner] = useState(null);
  const [contract, setContract] = useState(null);
  const [account, setAccount] = useState(null);
  const [isOwner, setIsOwner] = useState(false);
  const [recentActivity, setRecentActivity] = useState([]);
  const listenersRef = useRef({});

  const connectWallet = async () => {
    console.log("connecting to");
    if (!window.ethereum) {
      alert("Please install MetaMask!");
      return;
    }

    try {
      const tempProvider = new ethers.BrowserProvider(window.ethereum);
      const accounts = await tempProvider.send("eth_requestAccounts", []);
      const tempSigner = await tempProvider.getSigner();
      const tempContract = await createContract(tempProvider, tempSigner);
      const ownerAddress = await tempContract.owner();
      setProvider(tempProvider);
      setSigner(tempSigner);
      setContract(tempContract);
      setAccount(accounts[0]);
      setIsOwner(ownerAddress.toLowerCase() === accounts[0].toLowerCase());
    } catch (error) {
      alert(error.message || "Unable to connect to the charity contract.");
    }
  };

  const setupContractListeners = (contractInstance) => {
    if (!contractInstance) return;

    try {
      if (!listenersRef.current.charityRegistered) {
        const handler = (charityId, name, wallet) => {
          const newActivity = {
            type: "CharityRegistered",
            message: `New charity registered: ${name}`,
            timestamp: new Date().toLocaleTimeString(),
          };
          setRecentActivity((prev) => [newActivity, ...prev].slice(0, 50));
        };
        contractInstance.on("CharityRegistered", handler);
        listenersRef.current.charityRegistered = handler;
      }

      if (!listenersRef.current.charityStatusChanged) {
        const handler = (charityId, isActive) => {
          const newActivity = {
            type: "CharityStatusChanged",
            message: `Charity #${charityId} ${isActive ? "activated" : "deactivated"}`,
            timestamp: new Date().toLocaleTimeString(),
          };
          setRecentActivity((prev) => [newActivity, ...prev].slice(0, 50));
        };
        contractInstance.on("CharityStatusChanged", handler);
        listenersRef.current.charityStatusChanged = handler;
      }

      if (!listenersRef.current.platformFeeUpdated) {
        const handler = (newFee) => {
          const newActivity = {
            type: "PlatformFeeUpdated",
            message: `Platform fee updated to ${Number(newFee) / 100}%`,
            timestamp: new Date().toLocaleTimeString(),
          };
          setRecentActivity((prev) => [newActivity, ...prev].slice(0, 50));
        };
        contractInstance.on("PlatformFeeUpdated", handler);
        listenersRef.current.platformFeeUpdated = handler;
      }

      if (!listenersRef.current.donationReceived) {
        const handler = (charityId, campaignId, donor, amount) => {
          const newActivity = {
            type: "DonationReceived",
            message: `${ethers.formatEther(amount)} ETH donated to ${Number(campaignId) === 0 ? "charity" : "campaign"} #${Number(campaignId) || Number(charityId)}`,
            timestamp: new Date().toLocaleTimeString(),
          };
          setRecentActivity((prev) => [newActivity, ...prev].slice(0, 50));
        };
        contractInstance.on("DonationReceived", handler);
        listenersRef.current.donationReceived = handler;
      }
    } catch (err) {
      console.warn("Error setting up contract listeners:", err);
    }
  };

  const disconnectWallet = () => {
    setProvider(null);
    setSigner(null);
    setContract(null);
    setAccount(null);
    setIsOwner(false);
  };

  useEffect(() => {
    const tryAutoConnect = async () => {
      if (!window.ethereum) return;

      try {
        const tempProvider = new ethers.BrowserProvider(window.ethereum);
        const accounts = await tempProvider.send("eth_accounts", []);

        if (accounts.length === 0) return;

        const tempSigner = await tempProvider.getSigner();
        const tempContract = await createContract(tempProvider, tempSigner);
        const ownerAddress = await tempContract.owner();

        setProvider(tempProvider);
        setSigner(tempSigner);
        setContract(tempContract);
        setAccount(accounts[0]);
        setIsOwner(ownerAddress.toLowerCase() === accounts[0].toLowerCase());
      } catch (error) {
        console.warn("Wallet auto-connect skipped:", error.message || error);
      }
    };

    tryAutoConnect();
  }, []);

  // Attach listeners whenever contract changes; cleanup previous handlers
  useEffect(() => {
    if (!contract) return;
    setupContractListeners(contract);

    const current = contract;
    return () => {
      try {
        if (!current) return;
        if (listenersRef.current.charityRegistered)
          current.off("CharityRegistered", listenersRef.current.charityRegistered);
        if (listenersRef.current.charityStatusChanged)
          current.off("CharityStatusChanged", listenersRef.current.charityStatusChanged);
        if (listenersRef.current.platformFeeUpdated)
          current.off("PlatformFeeUpdated", listenersRef.current.platformFeeUpdated);
        if (listenersRef.current.donationReceived)
          current.off("DonationReceived", listenersRef.current.donationReceived);
        listenersRef.current = {};
      } catch (err) {
        console.warn("Error removing contract listeners:", err);
      }
    };
  }, [contract]);

  return (
    <WalletContext.Provider
      value={{
        provider,
        signer,
        contract,
        account,
        isOwner,
        recentActivity,
        connectWallet,
        disconnectWallet,
      }}
    >
      {children}
    </WalletContext.Provider>
  );
};

export const useWallet = () => useContext(WalletContext);
